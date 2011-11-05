package org.libsdl.app;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.*;

import android.app.*;
import android.content.*;
import android.view.*;
import android.os.*;
import android.util.DisplayMetrics;
import android.util.Log;
import android.graphics.*;
import android.text.method.*;
import android.text.*;
import android.media.*;
import android.hardware.*;
import android.content.*;
import android.content.res.AssetManager;

public class Lincity4droidActivity extends Activity {

	/**
    Simple nativeInit() runnable
*/
class UnpackAssets implements Runnable {

    public void run() {
	    String fullPath = "/mnt/sdcard/lincity/data/";
	    File dir = new File(fullPath);
	    if (!dir.exists()) 
	    {    	
	    	copyFileOrDir("");
	        String fullPath2 = "/mnt/sdcard/lincity/";
	        File dir2 = new File(fullPath2);  
	        if (!dir2.exists()) 
	        	dir2.mkdir(); 
	        dir.mkdir();
	    	copyFileOrDir("");
	    }	    	
    	alertDialog.cancel();
    }
}	
	
    // Main components
    private static Lincity4droidActivity mSingleton;
    private static SDLSurface mSurface;

    // Audio
    private static Thread mAudioThread;
    private static AudioTrack mAudioTrack;

	public static Thread mUnpackAssetsThread;    
	public static ProgressDialog  alertDialog;
	
	private PowerManager.WakeLock wl;
    
	private int countloader = 0;
    // Assets part
    private void copyFileOrDir(String path) {
    	
    	
        AssetManager assetManager = this.getAssets();
        String assets[] = null;
        try {
        	
        	String pathassets = path;
        	if (pathassets.startsWith("/"))
        	{
        		pathassets = pathassets.substring(1);
        	}
        	
            assets = assetManager.list(pathassets);
            if (assets.length == 0) {
                copyFile(path);
            } else {
                String fullPath = "/mnt/sdcard/lincity/data/" + path;
                File dir = new File(fullPath);
                if (!dir.exists())
                    dir.mkdir();
                for (int i = 0; i < assets.length; ++i) {
                    copyFileOrDir(path + "/" + assets[i]);
                }
            }
        } catch (IOException ex) {
            Log.e("tag", "I/O Exception", ex);
        }
    }

    private void copyFile(String filename) {
        AssetManager assetManager = this.getAssets();

        InputStream in = null;
        OutputStream out = null;
        try {
        	String filenameassets = filename;
        	if (filenameassets.startsWith("/"))
        	{
        		filenameassets = filenameassets.substring(1);
        	}
            in = assetManager.open(filenameassets);
            String newFileName = "/mnt/sdcard/lincity/data/" + filename;
            out = new FileOutputStream(newFileName);

            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
        } catch (Exception e) {
            Log.e("tag", e.getMessage());
        }
        countloader++;
        //double f = ((double) countloader) * 10000. / 1612.;
        //alertDialog.setProgress((int)f);
        alertDialog.setProgress(countloader);
        //Log.v("Lincity4droid", "Unpacking: "+countloader+" "+filename);

    }  
    
    // Load the .so
    static {
        System.loadLibrary("SDL");
        System.loadLibrary("SDL_image");
        System.loadLibrary("SDL_mixer");
        System.loadLibrary("SDL_ttf");
        System.loadLibrary("SDL_gfx");
        System.loadLibrary("physfs");
        System.loadLibrary("xml2");
        System.loadLibrary("main");
    }

    // Setup
    protected void onCreate(Bundle savedInstanceState) {
    	//Copying assets to /mnt/sdcard/lincity/data
         
        //Log.v("SDL", "onCreate()");

              
        
        
        // So we can call stuff from static callbacks
        mSingleton = this;

        /*
	    String fullPath = "/mnt/sdcard/lincity/data/";
	    File dir = new File(fullPath);
	    if (!dir.exists()) 
	    {
	    	ProgressDialog  alertDialog;
	        alertDialog = new ProgressDialog(this);
	        alertDialog.setTitle("Lincity4droid");
	        alertDialog.setMessage("Please wait while data are being unpacked.");
	        alertDialog.show();  
	        
	        String fullPath2 = "/mnt/sdcard/lincity/";
	        File dir2 = new File(fullPath2);  
	        if (!dir2.exists()) 
	        	dir2.mkdir(); 
	        dir.mkdir();
	    	copyFileOrDir("");
	    	
	        alertDialog.cancel();
	    }
	    */
	    
/*
        alertDialog = new ProgressDialog(this);
        alertDialog.setTitle("Lincity4droid");
        alertDialog.setMessage("Please wait while data are being unpacked.");
        alertDialog.show();
  */      
        PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        wl = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK, "DoNotDimScreen");
        wl.acquire();
        alertDialog = new ProgressDialog(this);
        alertDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        alertDialog.setMax(3252);
        alertDialog.setMessage("Please wait while data are being unpacked...");
        alertDialog.setCancelable(false);   
        alertDialog.show();
        
        // Now start up the C app thread
        if (mUnpackAssetsThread == null) {
        	mUnpackAssetsThread = new Thread(new UnpackAssets(), "UnpackAssetsThread"); 
        	mUnpackAssetsThread.start();       
        }        
          
        Log.v("SDL", "onCreate() Exiting");
        
        super.onCreate(savedInstanceState);
    }
  
    protected void onStart(){
        // Set up the surface
        mSurface = new SDLSurface(getApplication());
        setContentView(mSurface);
        SurfaceHolder holder = mSurface.getHolder();
        holder.setType(SurfaceHolder.SURFACE_TYPE_GPU);    	
        
        super.onStart();
    }
    
    // Events
    protected void onPause() {
        //Log.v("SDL", "onPause()");
        onNativePause();
    	
        super.onPause();
        

    }

    protected void onResume() {
    	
        // Set up the surface
        mSurface = new SDLSurface(getApplication());
        setContentView(mSurface);
        SurfaceHolder holder = mSurface.getHolder();
        holder.setType(SurfaceHolder.SURFACE_TYPE_GPU);    	
    	
    	onNativeResume();
        //Log.v("SDL", "onResume()");
        super.onResume();
    }

    // Messages from the SDLMain thread
    static int COMMAND_CHANGE_TITLE = 1;

    // Handler for the messages
    Handler commandHandler = new Handler() {
        public void handleMessage(Message msg) {
            if (msg.arg1 == COMMAND_CHANGE_TITLE) {
                setTitle((String)msg.obj);
            }
        }
    };

    // Send a message from the SDLMain thread
    void sendCommand(int command, Object data) {
        Message msg = commandHandler.obtainMessage();
        msg.arg1 = command;
        msg.obj = data;
        commandHandler.sendMessage(msg);
    }

    // C functions we call
    public static native void nativeInit(int w,int h);
    public static native void nativeQuit();
    public static native void onNativeResize(int x, int y, int format);
    public static native void onNativeKeyDown(int keycode);
    public static native void onNativeKeyUp(int keycode);
    public static native void onNativeTouch(int action, float x, 
                                            float y, float p);
    public static native void onNativeAccel(float x, float y, float z);
    public static native void nativeRunAudioThread();
    public static native void onNativePause();
    public static native void onNativeResume();


    // Java functions called from C

    public static boolean createGLContext(int majorVersion, int minorVersion) {
        return mSurface.initEGL(majorVersion, minorVersion);
    }

    public static void flipBuffers() {
        mSurface.flipEGL();
    }

    public static void setActivityTitle(String title) {
        // Called from SDLMain() thread and can't directly affect the view
        mSingleton.sendCommand(COMMAND_CHANGE_TITLE, title);
    }

	public static Context getContext() {
		return mSingleton;
	}

    // Audio
    private static Object buf;
    
    public static Object audioInit(int sampleRate, boolean is16Bit, boolean isStereo, int desiredFrames) {
        int channelConfig = isStereo ? AudioFormat.CHANNEL_CONFIGURATION_STEREO : AudioFormat.CHANNEL_CONFIGURATION_MONO;
        int audioFormat = is16Bit ? AudioFormat.ENCODING_PCM_16BIT : AudioFormat.ENCODING_PCM_8BIT;
        int frameSize = (isStereo ? 2 : 1) * (is16Bit ? 2 : 1);
        
        Log.v("SDL", "SDL audio: wanted " + (isStereo ? "stereo" : "mono") + " " + (is16Bit ? "16-bit" : "8-bit") + " " + ((float)sampleRate / 1000f) + "kHz, " + desiredFrames + " frames buffer");
        
        // Let the user pick a larger buffer if they really want -- but ye
        // gods they probably shouldn't, the minimums are horrifyingly high
        // latency already
        desiredFrames = Math.max(desiredFrames, (AudioTrack.getMinBufferSize(sampleRate, channelConfig, audioFormat) + frameSize - 1) / frameSize);
        
        mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate,
                channelConfig, audioFormat, desiredFrames * frameSize, AudioTrack.MODE_STREAM);
        
        audioStartThread();
        
        Log.v("SDL", "SDL audio: got " + ((mAudioTrack.getChannelCount() >= 2) ? "stereo" : "mono") + " " + ((mAudioTrack.getAudioFormat() == AudioFormat.ENCODING_PCM_16BIT) ? "16-bit" : "8-bit") + " " + ((float)mAudioTrack.getSampleRate() / 1000f) + "kHz, " + desiredFrames + " frames buffer");
        
        if (is16Bit) {
            buf = new short[desiredFrames * (isStereo ? 2 : 1)];
        } else {
            buf = new byte[desiredFrames * (isStereo ? 2 : 1)]; 
        }
        
        mAudioTrack.setStereoVolume(1.f, 1.f);
        return buf;
    }
    
    public static void audioStartThread() {
        mAudioThread = new Thread(new Runnable() {
            public void run() {
                mAudioTrack.play();
                nativeRunAudioThread();
            }
        });
        
        // I'd take REALTIME if I could get it!
        mAudioThread.setPriority(Thread.MAX_PRIORITY);
        mAudioThread.start();
    }
    
    public static void audioWriteShortBuffer(short[] buffer) {
        for (int i = 0; i < buffer.length; ) {
            int result = mAudioTrack.write(buffer, i, buffer.length - i);
            if (result > 0) {
                i += result;
            } else if (result == 0) {
                try {
                    Thread.sleep(1);
                } catch(InterruptedException e) {
                    // Nom nom
                }
            } else {
                Log.w("SDL", "SDL audio: error return from write(short)");
                return;
            }
        }
    }
    
    public static void audioWriteByteBuffer(byte[] buffer) {
        for (int i = 0; i < buffer.length; ) {
            int result = mAudioTrack.write(buffer, i, buffer.length - i);
            if (result > 0) {
                i += result;
            } else if (result == 0) {
                try {
                    Thread.sleep(1);
                } catch(InterruptedException e) {
                    // Nom nom
                }
            } else {
                Log.w("SDL", "SDL audio: error return from write(short)");
                return;
            }
        }
    }

    public static void audioQuit() {
        if (mAudioThread != null) {
            try {
                mAudioThread.join();
            } catch(Exception e) {
                Log.v("SDL", "Problem stopping audio thread: " + e);
            }
            mAudioThread = null;

            //Log.v("SDL", "Finished waiting for audio thread");
        }

        if (mAudioTrack != null) {
            mAudioTrack.stop();
            mAudioTrack = null;
        }
    }
}

/**
    Simple nativeInit() runnable
*/
class SDLMain implements Runnable {
	private int w;
	private int h;
	public SDLMain(int w,int h)
	{
		this.w = w;
		this.h = h;
	}
	
    public void run() {

    	try {
			Lincity4droidActivity.mUnpackAssetsThread.join();
			Lincity4droidActivity.alertDialog.cancel();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
        // Runs SDL_main()
    	Lincity4droidActivity.nativeInit(w,h);

        //Log.v("SDL", "SDL thread terminated");
    }
}


/**
    SDLSurface. This is what we draw on, so we need to know when it's created
    in order to do anything useful. 

    Because of this, that's where we set up the SDL thread
*/
class SDLSurface extends SurfaceView implements SurfaceHolder.Callback, 
    View.OnKeyListener, View.OnTouchListener, SensorEventListener  {

    // This is what SDL runs in. It invokes SDL_main(), eventually
    public static Thread mSDLThread;    
    
    // EGL private objects
    private EGLContext  mEGLContext;
    private EGLSurface  mEGLSurface;
    private EGLDisplay  mEGLDisplay;

    // Sensors
    private static SensorManager mSensorManager;

    // Startup    
    public SDLSurface(Context context) {
        super(context);
        getHolder().addCallback(this); 
    
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();
        setOnKeyListener(this); 
        setOnTouchListener(this);   

        mSensorManager = (SensorManager)context.getSystemService("sensor");  
    }

    // Called when we have a valid drawing surface
    public void surfaceCreated(SurfaceHolder holder) {
        //Log.v("SDL", "surfaceCreated()");

        enableSensor(Sensor.TYPE_ACCELEROMETER, true);
    }

    // Called when we lose the surface
    public void surfaceDestroyed(SurfaceHolder holder) {
        //Log.v("SDL", "surfaceDestroyed()");

        // Send a quit message to the application
    	
    	Lincity4droidActivity.nativeQuit();

        // Now wait for the SDL thread to quit
        if (mSDLThread != null) {
            try {
                mSDLThread.join();
            } catch(Exception e) {
                Log.v("SDL", "Problem stopping thread: " + e);
            }
            mSDLThread = null;

            //Log.v("SDL", "Finished waiting for SDL thread");
        }

        enableSensor(Sensor.TYPE_ACCELEROMETER, false);
        
    }

    // Called when the surface is resized
    public void surfaceChanged(SurfaceHolder holder,
                               int format, int width, int height) {
        //Log.v("SDL", "surfaceChanged()");

        int sdlFormat = 0x85151002; // SDL_PIXELFORMAT_RGB565 by default
        switch (format) {
        case PixelFormat.A_8:
            Log.v("SDL", "pixel format A_8");
            break;
        case PixelFormat.LA_88:
            Log.v("SDL", "pixel format LA_88");
            break;
        case PixelFormat.L_8:
            Log.v("SDL", "pixel format L_8");
            break;
        case PixelFormat.RGBA_4444:
            Log.v("SDL", "pixel format RGBA_4444");
            sdlFormat = 0x85421002; // SDL_PIXELFORMAT_RGBA4444
            break;
        case PixelFormat.RGBA_5551:
            Log.v("SDL", "pixel format RGBA_5551");
            sdlFormat = 0x85441002; // SDL_PIXELFORMAT_RGBA5551
            break;
        case PixelFormat.RGBA_8888:
            Log.v("SDL", "pixel format RGBA_8888");
            sdlFormat = 0x86462004; // SDL_PIXELFORMAT_RGBA8888
            break;
        case PixelFormat.RGBX_8888:
            Log.v("SDL", "pixel format RGBX_8888");
            sdlFormat = 0x86262004; // SDL_PIXELFORMAT_RGBX8888
            break;
        case PixelFormat.RGB_332:
            Log.v("SDL", "pixel format RGB_332");
            sdlFormat = 0x84110801; // SDL_PIXELFORMAT_RGB332
            break;
        case PixelFormat.RGB_565:
            Log.v("SDL", "pixel format RGB_565");
            sdlFormat = 0x85151002; // SDL_PIXELFORMAT_RGB565
            break;
        case PixelFormat.RGB_888:
            Log.v("SDL", "pixel format RGB_888");
            // Not sure this is right, maybe SDL_PIXELFORMAT_RGB24 instead?
            sdlFormat = 0x86161804; // SDL_PIXELFORMAT_RGB888
            break;
        default:
            Log.v("SDL", "pixel format unknown " + format);
            break;
        }
        Lincity4droidActivity.onNativeResize(width, height, sdlFormat);

       
        // Now start up the C app thread
        if (mSDLThread == null) {
            mSDLThread = new Thread(new SDLMain(width,height), "SDLThread"); 
            mSDLThread.start();       
        }
    }

    // unused
    public void onDraw(Canvas canvas) {}


    // EGL functions
    public boolean initEGL(int majorVersion, int minorVersion) {
        Log.v("SDL", "Starting up OpenGL ES " + majorVersion + "." + minorVersion);

        try {
            EGL10 egl = (EGL10)EGLContext.getEGL();

            EGLDisplay dpy = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);

            int[] version = new int[2];
            egl.eglInitialize(dpy, version);

            int EGL_OPENGL_ES_BIT = 1;
            int EGL_OPENGL_ES2_BIT = 4;
            int renderableType = 0;
            if (majorVersion == 2) {
                renderableType = EGL_OPENGL_ES2_BIT;
            } else if (majorVersion == 1) {
                renderableType = EGL_OPENGL_ES_BIT;
            }
            int[] configSpec = {
                //EGL10.EGL_DEPTH_SIZE,   16,
                EGL10.EGL_RENDERABLE_TYPE, renderableType,
                EGL10.EGL_NONE
            };
            EGLConfig[] configs = new EGLConfig[1];
            int[] num_config = new int[1];
            if (!egl.eglChooseConfig(dpy, configSpec, configs, 1, num_config) || num_config[0] == 0) {
                Log.e("SDL", "No EGL config available");
                return false;
            }
            EGLConfig config = configs[0];

            EGLContext ctx = egl.eglCreateContext(dpy, config, EGL10.EGL_NO_CONTEXT, null);
            if (ctx == EGL10.EGL_NO_CONTEXT) {
                Log.e("SDL", "Couldn't create context");
                return false;
            }

            EGLSurface surface = egl.eglCreateWindowSurface(dpy, config, this, null);
            if (surface == EGL10.EGL_NO_SURFACE) {
                Log.e("SDL", "Couldn't create surface");
                return false;
            }

            if (!egl.eglMakeCurrent(dpy, surface, surface, ctx)) {
                Log.e("SDL", "Couldn't make context current");
                return false;
            }

            mEGLContext = ctx;
            mEGLDisplay = dpy;
            mEGLSurface = surface;

        } catch(Exception e) {
            Log.v("SDL", e + "");
            for (StackTraceElement s : e.getStackTrace()) {
                Log.v("SDL", s.toString());
            }
        }

        return true;
    }

    // EGL buffer flip
    public void flipEGL() {
        try {
            EGL10 egl = (EGL10)EGLContext.getEGL();

            egl.eglWaitNative(EGL10.EGL_NATIVE_RENDERABLE, null);

            // drawing here

            egl.eglWaitGL();

            egl.eglSwapBuffers(mEGLDisplay, mEGLSurface);

            
        } catch(Exception e) {
            Log.v("SDL", "flipEGL(): " + e);
            for (StackTraceElement s : e.getStackTrace()) {
                Log.v("SDL", s.toString());
            }
        }
    }

    // Key events
    public boolean onKey(View  v, int keyCode, KeyEvent event) {

        if (event.getAction() == KeyEvent.ACTION_DOWN) {
            //Log.v("SDL", "key down: " + keyCode);
        	Lincity4droidActivity.onNativeKeyDown(keyCode);
            return true;
        }
        else if (event.getAction() == KeyEvent.ACTION_UP) {
            //Log.v("SDL", "key up: " + keyCode);
        	Lincity4droidActivity.onNativeKeyUp(keyCode);
            return true;
        }
        
        return false;
    }

    
 

    
    // Touch events
    public boolean onTouch(View v, MotionEvent event) {
    
        int action = event.getAction();
        float x = event.getX();
        float y = event.getY();
        float p = event.getPressure();

        // TODO: Anything else we need to pass?        
        Lincity4droidActivity.onNativeTouch(action, x, y, p);
        return true;
    }

    // Sensor events
    public void enableSensor(int sensortype, boolean enabled) {
        // TODO: This uses getDefaultSensor - what if we have >1 accels?
        if (enabled) {
            mSensorManager.registerListener(this, 
                            mSensorManager.getDefaultSensor(sensortype), 
                            SensorManager.SENSOR_DELAY_GAME, null);
        } else {
            mSensorManager.unregisterListener(this, 
                            mSensorManager.getDefaultSensor(sensortype));
        }
    }
    
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // TODO
    }

    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
        	Lincity4droidActivity.onNativeAccel(event.values[0],
                                      event.values[1],
                                      event.values[2]);
        }
    }
}