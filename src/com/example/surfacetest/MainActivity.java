package com.example.surfacetest;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends Activity {

	MySurface2 surface;
	NativeRenderer renderer;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//setContentView(R.layout.activity_main);
		
		surface = new MySurface2(this);
		renderer = new NativeRenderer(surface.getHolder());
		renderer.test();
		setContentView(surface);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	@Override
	public boolean onContextItemSelected(MenuItem item)
	{
		Log.d("native", "item selected");
		
		return false;
	}

}
