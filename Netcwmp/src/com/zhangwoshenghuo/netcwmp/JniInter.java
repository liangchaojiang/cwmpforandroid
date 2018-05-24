package com.zhangwoshenghuo.netcwmp;

public class JniInter {

	static {
		System.loadLibrary("tr111");
		System.loadLibrary("cwmp");
	}
	public native int tr111test();
	public native int tr069launch();
}
