import com.github.xpenatan.jparser.builder.BuildMultiTarget;
import com.github.xpenatan.jparser.builder.targets.WindowsMSVCTarget;
import com.github.xpenatan.jparser.builder.tool.BuildToolListener;
import com.github.xpenatan.jparser.builder.tool.BuildToolOptions;
import com.github.xpenatan.jparser.builder.tool.BuilderTool;
import com.github.xpenatan.jparser.idl.IDLReader;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

public class Build {

    public static void main(String[] args) {
        String libName = "jWebGPU";
        String modulePrefix = "webgpu";
        String basePackage = "com.github.xpenatan.webgpu";
//        WindowsMSVCTarget.DEBUG_BUILD = true;

        BuildToolOptions op = new BuildToolOptions(libName, basePackage, modulePrefix, null, args);
        BuilderTool.build(op, new BuildToolListener() {
            @Override
            public void onAddTarget(BuildToolOptions op, IDLReader idlReader, ArrayList<BuildMultiTarget> targets) {
                String dawnPath = null;
                try {
                    dawnPath = new File(op.getModuleBuildPath() + "/../webgpu-dawn/build/dawn").getCanonicalPath().replace("\\", "/");
                } catch(IOException e) {
                    throw new RuntimeException(e);
                }

                if(op.windows64) {
                    try {
                        targets.add(getWindowTarget(op, dawnPath));
                    } catch(IOException e) {
                        throw new RuntimeException(e);
                    }
                }
//                if(op.teavm) {
//                    targets.add(getTeaVMTarget(op, idlReader));
//                }
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

    private static BuildMultiTarget getWindowTarget(BuildToolOptions op, String dawnPath) throws IOException {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String libBuildCPPPath = op.getModuleBuildCPPPath();
        String dawnIncludePath = dawnPath + "/install/build_windows_x64/include";
        String dawnLibPath = dawnPath + "/install/build_windows_x64/lib";

        // Compile glue code and link
        WindowsMSVCTarget linkTarget = new WindowsMSVCTarget();
        linkTarget.addJNIHeaders();
        linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
        linkTarget.headerDirs.add("-I" + dawnIncludePath);
        linkTarget.cppCompiler.add("/MD");
        linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
        linkTarget.linkerFlags.add(dawnLibPath + "/webgpu_dawn.lib");
        linkTarget.linkerFlags.add("dxguid.lib");
        linkTarget.linkerFlags.add("advapi32.lib");
//        linkTarget.linkerFlags.add("kernel32.lib");
//        linkTarget.linkerFlags.add("user32.lib");
//        linkTarget.linkerFlags.add("gdi32.lib");
//        linkTarget.linkerFlags.add("winspool.lib");
//        linkTarget.linkerFlags.add("shell32.lib");
//        linkTarget.linkerFlags.add("ole32.lib");
//        linkTarget.linkerFlags.add("oleaut32.lib");
//        linkTarget.linkerFlags.add("uuid.lib");
//        linkTarget.linkerFlags.add("comdlg32.lib");
        multiTarget.add(linkTarget);

        return multiTarget;
    }
}