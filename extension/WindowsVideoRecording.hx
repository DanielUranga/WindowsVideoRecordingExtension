package extension;

#if cpp
import cpp.Lib;
#elseif neko
import neko.Lib;
#end

import openfl.display.BitmapData;
import openfl.utils.ByteArray;
import lime.math.color.BGRA;
import lime.math.Rectangle;

class WindowsVideoRecording {

    public static function init (outputPath : String, videoWidth : Int, videoHeight : Int, videoFPS : Int, videoBitRate : Int) : Void {
        windowsvideorecording_init(outputPath, videoWidth, videoHeight, videoFPS, videoBitRate);
    }

    public static  function dispose () : Void {
        windowsvideorecording_dispose();
    }

    public static function drawFrame (bmp : BitmapData) : Void {
        var limeRectangle = new Rectangle ();
        limeRectangle.setTo (0, 0, bmp.width, bmp.height);
        var bytes = ByteArray.fromBytes (bmp.image.getPixels (limeRectangle, BGRA32));
        windowsvideorecording_write_frame((bytes:ByteArrayData).getData ());
    }

    private static var windowsvideorecording_init = Lib.load ("windowsvideorecording", "windowsvideorecording_init", 5);
    private static var windowsvideorecording_dispose = Lib.load ("windowsvideorecording", "windowsvideorecording_dispose", 0);
    private static var windowsvideorecording_write_frame = Lib.load("windowsvideorecording", "windowsvideorecording_write_frame", 1);

}
