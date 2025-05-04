import com.github.xpenatan.jparser.builder.BuildMultiTarget;
import com.github.xpenatan.jparser.builder.targets.EmscriptenTarget;
import com.github.xpenatan.jparser.builder.targets.WindowsMSVCTarget;
import com.github.xpenatan.jparser.builder.tool.BuildToolListener;
import com.github.xpenatan.jparser.builder.tool.BuildToolOptions;
import com.github.xpenatan.jparser.builder.tool.BuilderTool;
import com.github.xpenatan.jparser.idl.IDLReader;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

public class WGPUBuild {

    public static void main(String[] args) {
        String libName = "jWebGPU";
        String modulePrefix = "webgpu";
        String basePackage = "com.github.xpenatan.webgpu";
//        WindowsMSVCTarget.DEBUG_BUILD = true;

        BuildToolOptions op = new BuildToolOptions(libName, basePackage, modulePrefix, null, args);

        BuilderTool.build(op, new BuildToolListener() {
            @Override
            public void onAddTarget(BuildToolOptions op, IDLReader idlReader, ArrayList<BuildMultiTarget> targets) {
                String buildPath = null;
                try {
                    buildPath = new File(op.getModuleBuildPath() + "/../webgpu-wgpu/build").getCanonicalPath().replace("\\", "/");
                } catch(IOException e) {
                    throw new RuntimeException(e);
                }

                if(op.windows64) {
                    try {
                        targets.add(getWindowTarget(op, buildPath));
                    } catch(IOException e) {
                        throw new RuntimeException(e);
                    }
                }
                if(op.teavm) {
                    targets.add(getTeaVMTarget(op, idlReader, buildPath));
                }
//                if(op.linux64) {
//                    targets.add(getLinuxTarget(op));
//                }
//                if(op.mac64) {
//                    targets.add(getMacTarget(op, false));
//                }
//                if(op.macArm) {
//                    targets.add(getMacTarget(op, true));
//                }
//                if(op.android) {
//                    targets.add(getAndroidTarget(op));
//                }
//                if(op.iOS) {
//                    targets.add(getIOSTarget(op));
//                }
            }
        });
    }

    private static BuildMultiTarget getWindowTarget(BuildToolOptions op, String buildPath) throws IOException {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String libBuildCPPPath = op.getModuleBuildCPPPath();
        String includePath = buildPath + "/windows_x86_64/include";
        String libPath = buildPath + "/windows_x86_64/lib/wgpu_native.lib";

        WindowsMSVCTarget.DEBUG_BUILD = false;

        // Compile glue code and link
        WindowsMSVCTarget linkTarget = new WindowsMSVCTarget();
        linkTarget.addJNIHeaders();
        linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
        linkTarget.headerDirs.add("-I" + includePath);
        linkTarget.cppCompiler.add("/MD");
        linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
        linkTarget.linkerFlags.add(libPath);
        linkTarget.linkerFlags.add("ws2_32.lib");
        linkTarget.linkerFlags.add("userenv.lib");
        linkTarget.linkerFlags.add("ntdll.lib");
        linkTarget.linkerFlags.add("opengl32.lib");
        linkTarget.linkerFlags.add("d3dcompiler.lib");
        linkTarget.linkerFlags.add("ole32.lib");
        linkTarget.linkerFlags.add("propsys.lib");
        linkTarget.linkerFlags.add("runtimeobject.lib");
        linkTarget.linkerFlags.add("user32.lib");
        linkTarget.linkerFlags.add("gdi32.lib");
        linkTarget.linkerFlags.add("oleaut32.lib");
        multiTarget.add(linkTarget);

        return multiTarget;
    }

    private static BuildMultiTarget getTeaVMTarget(BuildToolOptions op, IDLReader idlReader, String buildPath) {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String includePath = buildPath + "/emdawnwebgpu/webgpu/include";
        String customSourceDir = op.getCustomSourceDir();

        String port = buildPath + "/emdawnwebgpu/emdawnwebgpu.port.py";
        String jsLib = buildPath + "/emdawnwebgpu/webgpu/src/webgpu-externs.js";

        EmscriptenTarget.DEBUG_BUILD = false;


        // Make a static library
//        EmscriptenTarget libTarget = new EmscriptenTarget(idlReader);
//        libTarget.additionalSourceDirs.add(customSourceDir);
//        libTarget.isStatic = true;
//        libTarget.compileGlueCode = false;
//        libTarget.headerDirs.add("-I" + includePath);
//        libTarget.headerDirs.add("-I" + customSourceDir);
//        libTarget.cppInclude.add(customSourceDir + "/*.cpp");
//        multiTarget.add(libTarget);

        // Compile glue code and link
        EmscriptenTarget linkTarget = new EmscriptenTarget(idlReader);
        linkTarget.headerDirs.add("-I" + includePath);
        linkTarget.headerDirs.add("-I" + customSourceDir);
        linkTarget.cppFlags.add("--use-port=" + port);
        linkTarget.cppFlags.add("-fsanitize=address");
        linkTarget.linkerFlags.add("--use-port=" + port);
        linkTarget.headerDirs.add("-include" + op.getCustomSourceDir() + "WebGPUCustom.h");
        linkTarget.linkerFlags.add("-sASYNCIFY=1");
        linkTarget.linkerFlags.add("-fsanitize=address");
        linkTarget.linkerFlags.add("--closure-args=--externs=" + jsLib);
        multiTarget.add(linkTarget);

        return multiTarget;
    }
}