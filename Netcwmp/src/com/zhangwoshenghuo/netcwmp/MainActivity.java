package com.zhangwoshenghuo.netcwmp;



import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;


public class MainActivity extends Activity {

	
	Button bt1,bt2;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        bt1 = (Button)findViewById(R.id.btest);
        bt2 = (Button)findViewById(R.id.blaunch);
        bt1.setOnClickListener(new OnClickListener(){
        JniInter jniInter = new JniInter();
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				
				Thread th = new Thread(new Runnable(){

					@Override
					public void run() {
						// TODO Auto-generated method stub
						jniInter.tr111test();
					}
					
				});
				th.start();

				
			}
        	
        });
        
        bt2.setOnClickListener(new OnClickListener(){
        	JniInter jniInter = new JniInter();
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				Thread th = new Thread(new Runnable(){

					@Override
					public void run() {
						// TODO Auto-generated method stub
						jniInter.tr069launch();
					}
					
				});
				th.start();

			}
        	
        });
        
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
