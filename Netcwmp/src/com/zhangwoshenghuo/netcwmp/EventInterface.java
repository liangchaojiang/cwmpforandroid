package com.zhangwoshenghuo.netcwmp;

import java.io.IOException;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class EventInterface extends Service{
	
	public static void reboot(){
		
		try {
			Process proc = Runtime.getRuntime().exec("reboot");
			proc.waitFor();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		return null;
	}

}
