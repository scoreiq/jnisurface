package com.example.surfacetest;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.view.SurfaceHolder;

public class DrawThread extends Thread {
	
	boolean isRunning;
	SurfaceHolder holder;
	Bitmap picture;
	Matrix matrix;
	long prevTime;
	
	public DrawThread(SurfaceHolder sh, Resources res)
	{
		isRunning = false;
		holder = sh;
		picture = BitmapFactory.decodeResource(res, R.drawable.img);
		
		// формируем матрицу преобразований для картинки
        matrix = new Matrix();
        matrix.postScale(1.0f, 1.0f);
        matrix.postTranslate(100.0f, 100.0f);
        prevTime = System.currentTimeMillis();
	}
	
	public void setRunning(boolean b)
	{
		isRunning = b;
	}
	
	@Override
	public void run()
	{
		Canvas canvas = null;
		
		while(isRunning)
		{
			long now = System.currentTimeMillis();
			float deltaSeconds = (now - prevTime)/1000.0f;
			try {
                // получаем объект Canvas и выполняем отрисовку
                canvas = holder.lockCanvas(null);
                synchronized (holder) {
                    canvas.drawColor(Color.GRAY);
                    
                    matrix.preRotate(45.0f * deltaSeconds, picture.getWidth() / 2, picture.getHeight() / 2);
                    canvas.drawBitmap(picture, matrix, null);
                    
                    prevTime = now;
                }
            } 
            finally {
                if (canvas != null) {
                    // отрисовка выполнена. выводим результат на экран
                    holder.unlockCanvasAndPost(canvas);
                }
            }
		}
	}
}
