package;

import openfl.display.Sprite;
import openfl.events.Event;
import openfl.geom.Point;
import openfl.Lib;
import openfl.display.BitmapData;
import openfl.geom.Matrix;
import openfl.geom.Rectangle;
import openfl.events.KeyboardEvent;
import extension.WindowsVideoRecording;

class Main extends Sprite {

	var dotPosition : Point;
	var recording : Bool;

	public function new () {
		super ();
		dotPosition = new Point(100, 100);
		recording = false;
		addEventListener(Event.ADDED_TO_STAGE, onAddedToStage);
	}

	function screenshot() : BitmapData {
		var width = Lib.current.stage.stageWidth;
		var height = Lib.current.stage.stageHeight;
		var bmp = new BitmapData(width, height);
		var position = new Rectangle(0, 0, width, height);
		bmp.draw(Lib.current.stage, new Matrix(), position, false);
		return bmp;
	}

	function saveImageToDisk(bmp : BitmapData) {
		var fo = sys.io.File.write("test.png", true);
		var encoded = bmp.encode(bmp.rect, new openfl.display.PNGEncoderOptions());
		fo.writeString(encoded.toString());
		fo.close();
	}

	function updateDotPosition() {
		dotPosition.x = (dotPosition.x + Lib.current.stage.mouseX) / 2;
		dotPosition.y = (dotPosition.y + Lib.current.stage.mouseY) / 2;
	}

	// Event listeners:

	function onAddedToStage(_) {
		addEventListener(Event.ENTER_FRAME, onEnterFrame);
		Lib.current.stage.addEventListener(KeyboardEvent.KEY_DOWN, onKeyDown);
	}

	function onRemovedFromStage(_) {
		removeEventListener(Event.ENTER_FRAME, onEnterFrame);
		Lib.current.stage.removeEventListener(KeyboardEvent.KEY_DOWN, onKeyDown);
	}

	function onEnterFrame(_) {
		updateDotPosition();
		var gfx = this.graphics;
		gfx.clear();
		gfx.beginFill(0x0000ff);
		gfx.drawCircle(dotPosition.x, dotPosition.y, 50);
		gfx.endFill();
		gfx.beginFill(0x00ff00);
		gfx.drawCircle(dotPosition.x, dotPosition.y, 40);
		gfx.endFill();
		gfx.beginFill(0xff0000);
		gfx.drawCircle(dotPosition.x, dotPosition.y, 20);
		gfx.endFill();
		if (recording)
		{
			WindowsVideoRecording.drawFrame(screenshot());
		}
	}

	function onKeyDown(_) {
		saveImageToDisk(screenshot());
		if (recording) {
			WindowsVideoRecording.dispose();
		}
		else {
			WindowsVideoRecording.init("test.wmv", Lib.current.stage.stageWidth, Lib.current.stage.stageHeight, 60, 800000);
		}
		recording = !recording;
	}

}
