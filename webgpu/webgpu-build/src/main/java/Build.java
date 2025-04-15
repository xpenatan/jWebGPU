import com.github.xpenatan.jparser.builder.BuildMultiTarget;
import com.github.xpenatan.jparser.builder.targets.WindowsMSVCTarget;
import com.github.xpenatan.jparser.builder.tool.BuildToolListener;
import com.github.xpenatan.jparser.builder.tool.BuildToolOptions;
import com.github.xpenatan.jparser.builder.tool.BuilderTool;
import com.github.xpenatan.jparser.idl.IDLReader;
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
                if(op.windows64) {
                    targets.add(getWindowTarget(op));
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

    private static BuildMultiTarget getWindowTarget(BuildToolOptions op) {
        BuildMultiTarget multiTarget = new BuildMultiTarget();
        String libBuildCPPPath = op.getModuleBuildCPPPath();
        String buildPath = op.getModuleBuildPath() + "/build";
        String buildDawnPath = op.getModuleBuildPath() + "/build/dawn";
        String libsPath = buildPath + "/libs";
        String dawnGenPathInclude = buildDawnPath + "/build_windows_x64/gen/include";
        String dawnPathInclude = buildDawnPath + "/include";

        // Compile glue code and link
        WindowsMSVCTarget linkTarget = new WindowsMSVCTarget();
        linkTarget.addJNIHeaders();
        linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
        linkTarget.headerDirs.add("-I" + dawnGenPathInclude);
        linkTarget.headerDirs.add("-I" + dawnPathInclude);
        linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
        linkTarget.linkerFlags.add(libsPath + "/windows_x64/webgpu_dawn.lib");
        multiTarget.add(linkTarget);

        return multiTarget;
    }
}