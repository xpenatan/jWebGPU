package com.github.xpenatan.webgpu.demo.app.demos.mesh;

import com.github.xpenatan.webgpu.WGPUBuffer;
import com.github.xpenatan.webgpu.WGPUBufferDescriptor;
import com.github.xpenatan.webgpu.WGPUBufferMapCallback;
import com.github.xpenatan.webgpu.WGPUBufferUsage;
import com.github.xpenatan.webgpu.WGPUCallbackMode;
import com.github.xpenatan.webgpu.WGPUCommandEncoder;
import com.github.xpenatan.webgpu.WGPUExtent3D;
import com.github.xpenatan.webgpu.WGPUMapAsyncStatus;
import com.github.xpenatan.webgpu.WGPUMapMode;
import com.github.xpenatan.webgpu.WGPUTexture;
import com.github.xpenatan.webgpu.WGPUTextureAspect;
import com.github.xpenatan.webgpu.WGPUTextureFormat;
import com.github.xpenatan.webgpu.WGPUTexelCopyBufferInfo;
import com.github.xpenatan.webgpu.WGPUTexelCopyTextureInfo;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * Optional desktop screenshot capture to speed up visual debugging.
 */
public class StudioFrameCapture {

    private final boolean enabled;
    private final int captureEvery = Math.max(1, Integer.getInteger("studio.capture.every", 1));
    private final int maxCaptures = Math.max(1, Integer.getInteger("studio.capture.max", 1));
    private final String captureDir = System.getProperty("studio.capture.path", "local-logs/screenshots");
    private final boolean shadowCapture;
    private final boolean shadowDiagnostics = Boolean.parseBoolean(System.getProperty("studio.capture.shadow.diagnostics", "true"));

    private WGPUBuffer readbackBuffer;
    private int frameCounter;
    private int captureCounter;
    private boolean mapInFlight;
    private boolean copyEncoded;

    private int width;
    private int height;
    private int paddedBytesPerRow;
    private int bufferSize;
    private WGPUTextureFormat format;

    public StudioFrameCapture(boolean shadowCapture) {
        boolean baseEnabled = Boolean.parseBoolean(System.getProperty("studio.capture", "false"));
        boolean shadowEnabled = Boolean.parseBoolean(System.getProperty("studio.capture.shadow", "false"));
        this.shadowCapture = shadowCapture;
        this.enabled = baseEnabled && (!shadowCapture || shadowEnabled);
    }

    public void encodeCopyIfNeeded(WGPUApp wgpu, WGPUCommandEncoder encoder, WGPUTexture sourceTexture, int sourceWidth, int sourceHeight) {
        copyEncoded = false;
        if(!enabled || sourceTexture == null || mapInFlight || captureCounter >= maxCaptures) {
            return;
        }

        frameCounter++;
        if(frameCounter % captureEvery != 0) {
            return;
        }

        WGPUTextureFormat srcFormat = sourceTexture.getFormat();
        if(!isSupportedFormat(srcFormat)) {
            System.out.println("Studio capture skipped: unsupported surface format " + srcFormat);
            return;
        }

        ensureReadbackBuffer(wgpu, sourceWidth, sourceHeight, srcFormat);

        WGPUTexelCopyTextureInfo source = WGPUTexelCopyTextureInfo.obtain();
        source.setTexture(sourceTexture);
        source.setMipLevel(0);
        source.getOrigin().set(0, 0, 0);
        source.setAspect(WGPUTextureAspect.All);

        WGPUTexelCopyBufferInfo destination = WGPUTexelCopyBufferInfo.obtain();
        destination.setBuffer(readbackBuffer);
        destination.getLayout().setOffset(0);
        destination.getLayout().setBytesPerRow(paddedBytesPerRow);
        destination.getLayout().setRowsPerImage(height);

        WGPUExtent3D copySize = WGPUExtent3D.obtain();
        copySize.setWidth(width);
        copySize.setHeight(height);
        copySize.setDepthOrArrayLayers(1);

        encoder.copyTextureToBuffer(source, destination, copySize);
        copyEncoded = true;
    }

    public boolean isShadowCaptureEnabled() {
        return shadowCapture;
    }

    public void mapAfterSubmit() {
        if(!copyEncoded || readbackBuffer == null || mapInFlight || captureCounter >= maxCaptures) {
            return;
        }

        mapInFlight = true;
        readbackBuffer.mapAsync(WGPUMapMode.Read, 0, bufferSize, WGPUCallbackMode.AllowProcessEvents, new WGPUBufferMapCallback() {
            @Override
            protected void onCallback(WGPUMapAsyncStatus status, String message) {
                try {
                    if(status != WGPUMapAsyncStatus.Success) {
                        System.out.println("Studio capture map failed: " + status + " message: " + message);
                        return;
                    }
                    saveMappedPixels();
                }
                finally {
                    readbackBuffer.unmap();
                    mapInFlight = false;
                }
            }
        });
    }

    public void dispose() {
        if(readbackBuffer != null) {
            readbackBuffer.release();
            readbackBuffer = null;
        }
    }

    private void ensureReadbackBuffer(WGPUApp wgpu, int width, int height, WGPUTextureFormat format) {
        int unpaddedBytesPerRow = width * 4;
        int padded = ((unpaddedBytesPerRow + 255) / 256) * 256;
        int requiredSize = padded * height;

        boolean needsNewBuffer = readbackBuffer == null
                || this.width != width
                || this.height != height
                || this.paddedBytesPerRow != padded
                || this.bufferSize != requiredSize
                || this.format != format;

        if(!needsNewBuffer) {
            return;
        }

        if(readbackBuffer != null) {
            readbackBuffer.release();
            readbackBuffer = null;
        }

        this.width = width;
        this.height = height;
        this.paddedBytesPerRow = padded;
        this.bufferSize = requiredSize;
        this.format = format;

        WGPUBufferDescriptor bufferDesc = WGPUBufferDescriptor.obtain();
        bufferDesc.setLabel("studio screenshot staging");
        bufferDesc.setSize(requiredSize);
        bufferDesc.setUsage(WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.MapRead));
        bufferDesc.setMappedAtCreation(false);

        readbackBuffer = new WGPUBuffer();
        wgpu.device.createBuffer(bufferDesc, readbackBuffer);
    }

    private void saveMappedPixels() {
        ByteBuffer mapped = ByteBuffer.allocateDirect(bufferSize);
        mapped.order(ByteOrder.LITTLE_ENDIAN);
        readbackBuffer.getConstMappedRange(0, bufferSize, mapped);

        byte[] rgba = new byte[width * height * 4];
        boolean bgra = format == WGPUTextureFormat.BGRA8Unorm || format == WGPUTextureFormat.BGRA8UnormSrgb;
        boolean depth = format == WGPUTextureFormat.Depth32Float;

        for(int y = 0; y < height; y++) {
            int srcRow = y * paddedBytesPerRow;
            for(int x = 0; x < width; x++) {
                int i = srcRow + x * 4;
                int dst = (y * width + x) * 4;
                int c0 = mapped.get(i) & 0xFF;
                int c1 = mapped.get(i + 1) & 0xFF;
                int c2 = mapped.get(i + 2) & 0xFF;
                int c3 = mapped.get(i + 3) & 0xFF;

                int r;
                int g;
                int b;
                int a;
                if(depth) {
                    float d = mapped.getFloat(i);
                    if(Float.isNaN(d) || Float.isInfinite(d)) d = 1.0f;
                    d = Math.max(0.0f, Math.min(1.0f, d));
                    int gray = (int)(d * 255.0f);
                    r = gray;
                    g = gray;
                    b = gray;
                    a = 255;
                }
                else if(bgra) {
                    b = c0;
                    g = c1;
                    r = c2;
                    a = c3;
                }
                else {
                    r = c0;
                    g = c1;
                    b = c2;
                    a = c3;
                }

                rgba[dst] = (byte)r;
                rgba[dst + 1] = (byte)g;
                rgba[dst + 2] = (byte)b;
                rgba[dst + 3] = (byte)a;
            }
        }

        if(depth && shadowDiagnostics) {
            printShadowDiagnostics(mapped);
        }

        try {
            Path outDir = Paths.get(captureDir);
            Files.createDirectories(outDir);
            String prefix = depth ? "studio-shadow-" : "studio-";
            Path outFile = outDir.resolve(String.format(prefix + "%04d.rgba", captureCounter));
            Files.write(outFile, rgba);
            System.out.println("Studio capture saved: " + outFile.toAbsolutePath());
            captureCounter++;
        }
        catch(IOException e) {
            System.out.println("Studio capture write failed: " + e.getMessage());
        }
    }

    private boolean isSupportedFormat(WGPUTextureFormat format) {
        return format == WGPUTextureFormat.BGRA8Unorm
                || format == WGPUTextureFormat.BGRA8UnormSrgb
                || format == WGPUTextureFormat.RGBA8Unorm
                || format == WGPUTextureFormat.RGBA8UnormSrgb
                || format == WGPUTextureFormat.Depth32Float;
    }

    private void printShadowDiagnostics(ByteBuffer mapped) {
        int minX = width;
        int minY = height;
        int maxX = -1;
        int maxY = -1;

        int usedPixels = 0;
        int edgePixels = 0;

        float minDepth = 1.0f;
        float maxDepth = 0.0f;

        // Treat near-clear depth as unused. This highlights actual caster/receiver occupancy.
        final float usedThreshold = 0.9995f;
        final int edgeMarginX = Math.max(2, width / 50);
        final int edgeMarginY = Math.max(2, height / 50);

        for(int y = 0; y < height; y++) {
            int srcRow = y * paddedBytesPerRow;
            for(int x = 0; x < width; x++) {
                int i = srcRow + x * 4;
                float d = mapped.getFloat(i);
                if(Float.isNaN(d) || Float.isInfinite(d)) {
                    continue;
                }
                d = Math.max(0.0f, Math.min(1.0f, d));
                if(d >= usedThreshold) {
                    continue;
                }

                usedPixels++;
                if(d < minDepth) minDepth = d;
                if(d > maxDepth) maxDepth = d;

                if(x < minX) minX = x;
                if(y < minY) minY = y;
                if(x > maxX) maxX = x;
                if(y > maxY) maxY = y;

                if(x < edgeMarginX || x >= width - edgeMarginX || y < edgeMarginY || y >= height - edgeMarginY) {
                    edgePixels++;
                }
            }
        }

        if(usedPixels == 0) {
            System.out.println("Studio shadow diagnostics: no used depth pixels detected (shadow map appears empty).");
            System.out.println("Studio shadow diagnostics recommendation: decrease lightDistance or tighten orthographic bounds around scene.");
            return;
        }

        float usedPercent = (usedPixels * 100.0f) / (width * height);
        float edgePercent = (edgePixels * 100.0f) / usedPixels;
        float depthSpan = maxDepth - minDepth;

        System.out.println(String.format(
                "Studio shadow diagnostics: used=%.2f%% bounds=[%d..%d, %d..%d] edgeClip=%.2f%% depth[min=%.4f max=%.4f span=%.4f]",
                usedPercent, minX, maxX, minY, maxY, edgePercent, minDepth, maxDepth, depthSpan
        ));

        if(edgePercent > 8.0f || minX <= edgeMarginX || minY <= edgeMarginY || maxX >= width - edgeMarginX - 1 || maxY >= height - edgeMarginY - 1) {
            System.out.println("Studio shadow diagnostics recommendation: expand orthographic shadow bounds or re-center light frustum.");
        }
        else if(usedPercent < 8.0f) {
            System.out.println("Studio shadow diagnostics recommendation: tighten orthographic bounds to increase shadow texel density.");
        }

        if(minDepth < 0.02f) {
            System.out.println("Studio shadow diagnostics recommendation: reduce shadow near-plane pressure (increase near or pull light back slightly).");
        }
        if(maxDepth > 0.98f) {
            System.out.println("Studio shadow diagnostics recommendation: increase far plane or reduce light distance to avoid depth clipping.");
        }
        if(depthSpan < 0.08f) {
            System.out.println("Studio shadow diagnostics recommendation: depth span is narrow; tighten frustum or increase caster spread for better precision.");
        }
    }
}


