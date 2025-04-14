import com.github.xpenatan.jparser.builder.BuildMultiTarget;
import com.github.xpenatan.jparser.builder.targets.AndroidTarget;
import com.github.xpenatan.jparser.builder.targets.EmscriptenTarget;
import com.github.xpenatan.jparser.builder.targets.LinuxTarget;
import com.github.xpenatan.jparser.builder.targets.MacTarget;
import com.github.xpenatan.jparser.builder.targets.WindowsMSVCTarget;
import com.github.xpenatan.jparser.builder.tool.BuildToolListener;
import com.github.xpenatan.jparser.builder.tool.BuildToolOptions;
import com.github.xpenatan.jparser.builder.tool.BuilderTool;
import com.github.xpenatan.jparser.idl.IDLHelper;
import com.github.xpenatan.jparser.idl.IDLPackageRenaming;
import com.github.xpenatan.jparser.idl.IDLReader;
import java.util.ArrayList;

public class Build {

    public static void main(String[] args) {
        String libName = "webgpu";
        String modulePrefix = "webgpu";
        String basePackage = "com.github.xpenatan.webgpu";
        String sourcePath =  "/build/webgpu";
//        WindowsMSVCTarget.DEBUG_BUILD = true;

        IDLHelper.cppConverter = idlType -> {
            if(idlType.equals("unsigned long long")) {
                return "uint64";
            }
            return null;
        };

        BuildToolOptions op = new BuildToolOptions(modulePrefix, libName, basePackage, sourcePath, args);
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
        String sourceDir = op.getSourceDir();

        // Compile glue code and link
        WindowsMSVCTarget linkTarget = new WindowsMSVCTarget();
        linkTarget.addJNIHeaders();
        linkTarget.headerDirs.add("-I" + sourceDir);
        linkTarget.headerDirs.add("-I" + op.getCustomSourceDir());
        linkTarget.cppInclude.add(libBuildCPPPath + "/src/jniglue/JNIGlue.cpp");
        linkTarget.linkerFlags.add(libBuildCPPPath + "/libs/windows/vc/webgpu64_.lib");
        multiTarget.add(linkTarget);

        return multiTarget;
    }
}