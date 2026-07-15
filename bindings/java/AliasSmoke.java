import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;

import org.verovio.lib.toolkit;

public class AliasSmoke {
    public static void main(String[] args) throws Exception {
        if (args.length != 6) {
            throw new IllegalArgumentException(
                "Usage: AliasSmoke JNI_LIBRARY RESOURCES TEXT_FONT MUSIC_FONT MUSIC_METADATA MEI");
        }
        System.load(Paths.get(args[0]).toAbsolutePath().toString());
        toolkit verovio = new toolkit(false);
        if (!verovio.setResourcePath(args[1])) throw new IllegalStateException("Could not load resources");
        String textFamily = verovio.registerTextFont(Files.readAllBytes(Paths.get(args[2])), "QS");
        String musicFamily = verovio.registerMusicFont(
            Files.readAllBytes(Paths.get(args[3])), readString(args[4]), "VM");
        if (!textFamily.equals("Verovio Test Ligature") || !musicFamily.equals("Verovio Test Music")) {
            throw new IllegalStateException("Unexpected canonical font family");
        }
        if (!verovio.loadData(readString(args[5]))) {
            throw new IllegalStateException(verovio.getLog());
        }
        String svg = verovio.renderToSVG(1);
        if (svg.contains("<text") || !svg.contains("font-family=\"QS\"")
            || !svg.contains("font-family=\"VM\"") || !svg.contains("text-8BFEB250B0FDDA0E-4-")) {
            throw new IllegalStateException("Aliased fonts were not used in rendered SVG");
        }
        verovio.delete();
    }

    private static String readString(String filename) throws Exception {
        return new String(Files.readAllBytes(Paths.get(filename)), StandardCharsets.UTF_8);
    }
}
