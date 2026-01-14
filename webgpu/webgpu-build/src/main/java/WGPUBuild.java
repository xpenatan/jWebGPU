import com.github.xpenatan.jParser.builder.BuildMultiTarget;
import com.github.xpenatan.jParser.builder.targets.AndroidTarget;
import com.github.xpenatan.jParser.builder.targets.EmscriptenTarget;
import com.github.xpenatan.jParser.builder.targets.LinuxTarget;
import com.github.xpenatan.jParser.builder.targets.MacTarget;
import com.github.xpenatan.jParser.builder.targets.WindowsMSVCTarget;
import com.github.xpenatan.jParser.builder.tool.BuildToolListener;
import com.github.xpenatan.jParser.builder.tool.BuildToolOptions;
import com.github.xpenatan.jParser.builder.tool.BuilderTool;
import com.github.xpenatan.jParser.core.JParser;
import com.github.xpenatan.jParser.idl.IDLReader;
import com.github.xpenatan.jParser.idl.IDLRenaming;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;

public class WGPUBuild {

    public static void main(String[] args) {
        String libName = "jWebGPU";
        String modulePrefix = "webgpu";
        String basePackage = "com.github.xpenatan.webgpu";

        JParser.CREATE_IDL_HELPER = false;

        BuildToolOptions.BuildToolParams data = new BuildToolOptions.BuildToolParams();
        data.libName = libName;
        data.idlName = libName;
        data.webModuleName = libName;
        data.packageName = basePackage;
        data.modulePrefix = modulePrefix;
        BuildToolOptions op = new BuildToolOptions(data, args);
        op.addAdditionalIDLRefPath(IDLReader.getIDLHelperFile());

        BuilderTool.build(op, new BuildToolListener() {
            @Override
            public void onAddTarget(BuildToolOptions op, IDLReader idlReader, ArrayList<BuildMultiTarget> targets) {
                String downloadPath = null;
                try {
                    downloadPath = new File(op.getModuleBuildPath() + "/../webgpu-download/build").getCanonicalPath().replace("\\", "/");
                } catch(IOException e) {
                    throw new RuntimeException(e);
                }
                if(op.containsArg("windows64")) {
                    targets.add(getWindowTarget(op, downloadPath));
                }
                if(op.containsArg("windows64_dawn")) {
                    targets.add(getWindowDawnTarget(op, downloadPath));
//                    targets.add(getWindowDawn2Target(op, downloadPath));
                }
                if(op.containsArg("teavm")) {
                    targets.add(getTeaVMTarget(op, idlReader, downloadPath));
                }
                if(op.containsArg("android")) {
                    targets.add(getAndroidTarget(op, downloadPath));
                }
                if(op.containsArg("linux64")) {
                    targets.add(getLinuxTarget(op, downloadPath));
                }
                if(op.containsArg("mac64")) {
                    targets.add(getMacTarget(op, downloadPath, false));
                }
                if(op.containsArg("macArm")) {
                    targets.add(getMacTarget(op, downloadPath, true));
                }
//                if(op.containsArg("iOS")) {
//                    targets.add(getIOSTarget(op));
//                }
            }
        }, new IDLRenaming() {
            @Override
            public String getIDLMethodName(String methodName) {
                char[] c = methodName.toCharArray();
                c[0] = Character.toLowerCase(c[0]);
                methodName = new String(c);
                return methodName;
            }
        });
    }

    private static BuildMultiTarget getWindowTarget(BuildToolOptions op, String downloadPath) {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String libBuildCPPPath = op.getModuleBuildCPPPath();

//        WindowsMSVCTarget.DEBUG_BUILD = true;
//        String wgpuPath = buildPath + "/windows_x86_64_debug";

        String wgpuPath = downloadPath + "/windows_x86_64";
        String webgpuIncludePath = wgpuPath + "/include";
        String libPath = wgpuPath + "/lib/wgpu_native.lib";
        String glfwIncludePath = downloadPath + "/GLFW";

        // Compile glue code and link
        WindowsMSVCTarget linkTarget = new WindowsMSVCTarget();
        linkTarget.addJNIHeaders();
        linkTarget.cppFlags.add("-std:c++17");
        linkTarget.libDirSuffix += "wgpu/";
        linkTarget.tempBuildDir += "_wgpu";

        linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
        linkTarget.headerDirs.add("-I" + webgpuIncludePath);
        linkTarget.headerDirs.add("-I" + glfwIncludePath);
        linkTarget.cppCompiler.add("/MD");
        linkTarget.cppCompiler.add("/EHsc");
        linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
        linkTarget.cppInclude.add(op.getCustomSourceDir() + "jWebGPU.cpp");
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
        linkTarget.linkerFlags.add("-DLL");
        multiTarget.add(linkTarget);

        return multiTarget;
    }

    private static BuildMultiTarget getWindowDawnTarget(BuildToolOptions op, String downloadPath) {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String libBuildCPPPath = op.getModuleBuildCPPPath();

        String dawnPath = downloadPath + "/dawn-x64";
        String webgpuIncludePath = dawnPath + "/include";
        String libPath = dawnPath + "/webgpu_dawn.lib";
        String glfwIncludePath = downloadPath + "/GLFW";

        // Compile glue code and link
        WindowsMSVCTarget linkTarget = new WindowsMSVCTarget();
        linkTarget.addJNIHeaders();
        linkTarget.cppFlags.add("-std:c++17");
        linkTarget.libDirSuffix += "dawn/";
        linkTarget.tempBuildDir += "_dawn";
        linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
        linkTarget.headerDirs.add("-I" + webgpuIncludePath);
        linkTarget.headerDirs.add("-I" + glfwIncludePath);
        linkTarget.cppCompiler.add("/MD");
        linkTarget.cppCompiler.add("/EHsc");
        linkTarget.cppCompiler.add("-DWEBGPU_DAWN");
        linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
        linkTarget.cppInclude.add(op.getCustomSourceDir() + "jWebGPU.cpp");
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
        linkTarget.linkerFlags.add("-DLL");
        multiTarget.add(linkTarget);

        Path headerSouce = Paths.get(dawnPath + "/webgpu.h");
        Path headerDestination = Paths.get(webgpuIncludePath + "/webgpu/webgpu.h");
        Path nativeFile = Paths.get(dawnPath + "/webgpu_dawn.dll");

        String dllOutputPath = op.getLibsDir() + "/" + linkTarget.libDirSuffix;
        Path nativeDestination = Paths.get(dllOutputPath + "/webgpu_dawn.dll");
        try {
            Files.createDirectories(headerDestination.getParent());
            Files.createDirectories(nativeDestination.getParent());
            Files.copy(headerSouce, headerDestination, java.nio.file.StandardCopyOption.REPLACE_EXISTING);
            Files.copy(nativeFile, nativeDestination, java.nio.file.StandardCopyOption.REPLACE_EXISTING);
        } catch(IOException e) {
            throw new RuntimeException(e);
        }
        return multiTarget;
    }

    private static BuildMultiTarget getWindowDawn2Target(BuildToolOptions op, String downloadPath) {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String libBuildCPPPath = op.getModuleBuildCPPPath();

        String dawnPath = downloadPath + "/dawn_windows_x64_shared";
        String webgpuIncludePath = dawnPath + "/include";
        String libPath = dawnPath + "/lib/webgpu_dawn.lib";
        String glfwIncludePath = downloadPath + "/GLFW";

        // Compile glue code and link
        WindowsMSVCTarget linkTarget = new WindowsMSVCTarget();
        linkTarget.addJNIHeaders();
        linkTarget.cppFlags.add("-std:c++17");
        linkTarget.libDirSuffix += "dawn/";
        linkTarget.tempBuildDir += "_dawn";
        linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
        linkTarget.headerDirs.add("-I" + webgpuIncludePath);
        linkTarget.headerDirs.add("-I" + glfwIncludePath);
        linkTarget.cppCompiler.add("/MD");
        linkTarget.cppCompiler.add("-DWEBGPU_DAWN");
        linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
        linkTarget.cppInclude.add(op.getCustomSourceDir() + "jWebGPU.cpp");
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
        linkTarget.linkerFlags.add("-DLL");
        multiTarget.add(linkTarget);

        Path headerSouce = Paths.get(dawnPath + "/webgpu.h");
//        Path headerDestination = Paths.get(webgpuIncludePath + "/webgpu/webgpu.h");
        Path nativeFile = Paths.get(dawnPath + "/bin/webgpu_dawn.dll");

        String dllOutputPath = op.getLibsDir() + "/" + linkTarget.libDirSuffix;
        Path nativeDestination = Paths.get(dllOutputPath + "/webgpu_dawn.dll");
        try {
//            Files.createDirectories(headerDestination.getParent());
            Files.createDirectories(nativeDestination.getParent());
//            Files.copy(headerSouce, headerDestination, java.nio.file.StandardCopyOption.REPLACE_EXISTING);
            Files.copy(nativeFile, nativeDestination, java.nio.file.StandardCopyOption.REPLACE_EXISTING);
        } catch(IOException e) {
            throw new RuntimeException(e);
        }
        return multiTarget;
    }

    private static BuildMultiTarget getLinuxTarget(BuildToolOptions op, String downloadPath) {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String libBuildCPPPath = op.getModuleBuildCPPPath();

        String wgpuPath = downloadPath + "/linux_x86_64";
        String webgpuIncludePath = wgpuPath + "/include";
        String libPath = wgpuPath + "/lib/libwgpu_native.a";
        String glfwIncludePath = downloadPath + "/GLFW";

        // Compile glue code and link
        LinuxTarget linkTarget = new LinuxTarget();
        linkTarget.addJNIHeaders();
        linkTarget.cppFlags.add("-std=c++17");
        linkTarget.cppFlags.add("-fPIC");
        linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
        linkTarget.headerDirs.add("-I" + webgpuIncludePath);
        linkTarget.headerDirs.add("-I" + glfwIncludePath);
        linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
        linkTarget.cppInclude.add(op.getCustomSourceDir() + "jWebGPU.cpp");
        linkTarget.linkerFlags.add(libPath);
        multiTarget.add(linkTarget);

        return multiTarget;
    }

    private static BuildMultiTarget getMacTarget(BuildToolOptions op, String downloadPath, boolean isArm) {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String libBuildCPPPath = op.getModuleBuildCPPPath();

        if(isArm) {
            String wgpuPath = downloadPath + "/macos_aarch64";
            String webgpuIncludePath = wgpuPath + "/include";
            String libPath = wgpuPath + "/lib/libwgpu_native.a";
            String glfwIncludePath = downloadPath + "/GLFW";

            // Compile glue code and link
            MacTarget linkTarget = new MacTarget(true);
            linkTarget.addJNIHeaders();
            linkTarget.cppFlags.add("-std=c++17");
            linkTarget.cppFlags.add("-fPIC");
            linkTarget.cppCompiler.add("-x");
            linkTarget.cppCompiler.add("objective-c++");
            linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
            linkTarget.headerDirs.add("-I" + webgpuIncludePath);
            linkTarget.headerDirs.add("-I" + glfwIncludePath);
            linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
            linkTarget.cppInclude.add(op.getCustomSourceDir() + "jWebGPU.cpp");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("Metal");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("Foundation");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("Cocoa");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("QuartzCore");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("CoreFoundation");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("IOKit");
            linkTarget.linkerFlags.add(libPath);
            multiTarget.add(linkTarget);
        }
        else {
            String wgpuPath = downloadPath + "/macos_x86_64";
            String webgpuIncludePath = wgpuPath + "/include";
            String libPath = wgpuPath + "/lib/libwgpu_native.a";
            String glfwIncludePath = downloadPath + "/GLFW";

            // Compile glue code and link
            MacTarget linkTarget = new MacTarget(false);
            linkTarget.addJNIHeaders();
            linkTarget.cppFlags.add("-std=c++17");
            linkTarget.cppFlags.add("-fPIC");
            linkTarget.cppCompiler.add("-x");
            linkTarget.cppCompiler.add("objective-c++");
            linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
            linkTarget.headerDirs.add("-I" + webgpuIncludePath);
            linkTarget.headerDirs.add("-I" + glfwIncludePath);
            linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
            linkTarget.cppInclude.add(op.getCustomSourceDir() + "jWebGPU.cpp");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("Metal");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("Foundation");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("Cocoa");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("QuartzCore");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("CoreFoundation");
            linkTarget.linkerFlags.add("-framework");
            linkTarget.linkerFlags.add("IOKit");
            linkTarget.linkerFlags.add(libPath);
            multiTarget.add(linkTarget);
        }

        return multiTarget;
    }

    private static BuildMultiTarget getTeaVMTarget(BuildToolOptions op, IDLReader idlReader, String downloadPath) {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String includePath = downloadPath + "/emdawnwebgpu_pkg/webgpu/include";
        String includeCPPPath = downloadPath + "/emdawnwebgpu_pkg/webgpu_cpp/include";
        String customSourceDir = op.getCustomSourceDir();

        String port = downloadPath + "/emdawnwebgpu_pkg/emdawnwebgpu.port.py";
        String jsLib = downloadPath + "/emdawnwebgpu_pkg/webgpu/src/webgpu-externs.js";

//        EmscriptenTarget.DEBUG_BUILD = false;

        // Compile glue code and link
        EmscriptenTarget linkTarget = new EmscriptenTarget();
        linkTarget.idlReader = idlReader;
        linkTarget.mainModuleName = "idl";
        linkTarget.cppInclude.add(customSourceDir + "**.cpp");
        linkTarget.cppFlags.add("-std=c++17");
        linkTarget.headerDirs.add("-I" + includePath);
        linkTarget.headerDirs.add("-I" + includeCPPPath);
        linkTarget.headerDirs.add("-I" + customSourceDir);
        linkTarget.headerDirs.add("-include" + op.getCustomSourceDir() + "jWebGPU.h");
        linkTarget.linkerFlags.add("-sSIDE_MODULE=2");
        linkTarget.linkerFlags.add("--use-port=" + port);
        linkTarget.linkerFlags.add("--closure-args=--externs=" + jsLib);
        linkTarget.linkerFlags.add("-lc++abi"); // C++ ABI (exceptions, thread_atexit, etc.)
        linkTarget.linkerFlags.add("-lc++"); // C++ STL (std::cout, std::string, etc.)
        linkTarget.linkerFlags.add("-lc"); // C standard library (fopen, fclose, printf, etc.)
        multiTarget.add(linkTarget);

        return multiTarget;
    }

    private static BuildMultiTarget getAndroidTarget(BuildToolOptions op, String downloadPath) {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String libBuildCPPPath = op.getModuleBuildCPPPath();

        AndroidTarget.ApiLevel apiLevel = AndroidTarget.ApiLevel.Android_10_29;
        ArrayList<AndroidTarget.Target> targets = new ArrayList<>();

        targets.add(AndroidTarget.Target.x86);
        targets.add(AndroidTarget.Target.x86_64);
        targets.add(AndroidTarget.Target.armeabi_v7a);
        targets.add(AndroidTarget.Target.arm64_v8a);


        for(int i = 0; i < targets.size(); i++) {
            AndroidTarget.Target target = targets.get(i);

            String arch = null;

            if(target == AndroidTarget.Target.x86_64) {
                arch = "x86_64";
            }
            if(target == AndroidTarget.Target.x86) {
                arch = "i686";
            }
            else if(target == AndroidTarget.Target.arm64_v8a) {
                arch = "aarch64";
            }
            else if(target == AndroidTarget.Target.armeabi_v7a) {
                arch = "armv7";
            }

            if(arch == null) continue;

            // x86_64
            String wgpuPath = downloadPath + "/android_" + arch;
            String webgpuIncludePath = wgpuPath + "/include";
            String libPath = wgpuPath + "/lib/libwgpu_native.a";

            AndroidTarget linkTarget = new AndroidTarget(target, apiLevel);
            linkTarget.addJNIHeaders();
            linkTarget.cppFlags.add("-std=c++17");
            linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
            linkTarget.headerDirs.add("-I" + webgpuIncludePath);
            linkTarget.headerDirs.add("-I" + libBuildCPPPath + "/src/jniglue");
            linkTarget.linkerFlags.add(libPath);
            linkTarget.cppCompiler.add("-fPIC");
            linkTarget.linkerFlags.add("-landroid");
            linkTarget.linkerFlags.add("-llog");
            linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
            linkTarget.cppInclude.add(op.getCustomSourceDir() + "jWebGPU.cpp");
            linkTarget.linkerFlags.add("-Wl,-z,max-page-size=16384");
            multiTarget.add(linkTarget);
        }
        return multiTarget;
    }
}