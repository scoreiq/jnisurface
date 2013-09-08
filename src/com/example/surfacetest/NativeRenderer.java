package com.example.surfacetest;

import java.nio.ByteBuffer;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.util.Log;
import android.view.SurfaceHolder;

public class NativeRenderer {
	static {
		System.loadLibrary("SurfaceTest");
	}
	
	SurfaceHolder holder;
	
	private final int W = 100;
	private final int H = 100;
	
	Bitmap 		bitmap;
	ByteBuffer 	buffer;
	
	public native void test();
	public native void drawNoise();
	
	public NativeRenderer(SurfaceHolder holder)
	{
		this.holder = holder;
	}
	
	public ByteBuffer initSurface()
	{
		Log.d("Java side", "initSurface");
		synchronized (this) {
			bitmap = Bitmap.createBitmap(W, H, Bitmap.Config.RGB_565);
			buffer = ByteBuffer.allocateDirect(W*H*2);
			
			Log.d("Java side", "initSurface: before return");
			return buffer;
		}
	}
	
	
	private void surfaceRender() {
	  synchronized (this) {
	    try {
	      Canvas c = holder.lockCanvas(null);
	      bitmap.copyPixelsFromBuffer(buffer);
	      c.drawBitmap(bitmap, 0, 0, null);
	      holder.unlockCanvasAndPost(c);
	    } catch (Exception e) {
	    }
	  }
	}

	private void surfaceRelease() {
	  synchronized (this) {
	    bitmap.recycle();
	    bitmap = null;
	    buffer = null;
	  }
	}
}
