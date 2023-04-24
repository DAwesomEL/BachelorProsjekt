package com.infintro.loomocart;


import android.content.Context;


import android.hardware.usb.UsbDevice;
import android.os.Handler;
import android.app.Activity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.widget.TextView;
import android.view.View;
import android.widget.Button;
import android.util.Log;

import me.aflak.arduino.Arduino;
import me.aflak.arduino.ArduinoListener;

public class MainActivity extends Activity implements ArduinoListener {
    private Arduino arduino;
    private TextView textView;


    private Button mGo;
    private Button mStop;
    private Button mForward;
    private Button mBackward;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView = findViewById(R.id.textView);
        textView.setMovementMethod(new ScrollingMovementMethod());
        arduino = new Arduino(this);
        display("Please plug an Arduino via OTG.\nOn some devices you will have to enable OTG Storage in the phone's settings.\n\n");


        initView();
        initListener();
        Arduino arduino = new Arduino(getApplicationContext()); //  https://stackoverflow.com/questions/3572463/what-is-context-on-android
        arduino.addVendorId(2341);

    }




    @Override
    protected void onStart() {
        super.onStart();
        arduino.setArduinoListener(this);


    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        arduino.unsetArduinoListener();
        arduino.close();
    }

    @Override
    public void onArduinoAttached(UsbDevice device) {
        display("Arduino attached!");
        arduino.open(device);

    }

    @Override
    public void onArduinoDetached() {
        display("Arduino detached");
    }

    @Override
    public void onArduinoMessage(byte[] bytes) {
        //display("> "+new String(bytes));
    }

    @Override
    public void onArduinoOpened() {
        //String str = "Hello World !";
        //arduino.send(str.getBytes());
    }

    @Override
    public void onUsbPermissionDenied() {
        display("Permission denied... New attempt in 3 sec");
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                arduino.reopen();
            }
        }, 3000);
    }

    public void display(final String message){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                textView.append(message+"\n");
            }
        });
    }



    private void initView() {

        mGo = findViewById(R.id.Go);
        mStop = findViewById(R.id.Stop);
        mForward = findViewById(R.id.Forward);
        mBackward = findViewById(R.id.Backward);

    }

    private void initListener() {
        mGo.setOnClickListener(goListener);
        mStop.setOnClickListener(stopListener);
        mForward.setOnClickListener(forwardListener);
        mBackward.setOnClickListener(backwardListener);


    }




    private View.OnClickListener forwardListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            arduino.send("F".getBytes());
        }
    };


    private View.OnClickListener backwardListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            arduino.send("B".getBytes());
        }
    };

    private View.OnClickListener goListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            arduino.send("G".getBytes());
        }
    };


    private View.OnClickListener stopListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            arduino.send("S".getBytes());
        }
    };


}















/*






import android.app.Activity;
import android.graphics.SurfaceTexture;
import android.os.Bundle;
import android.util.Log;
import android.view.TextureView;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;


import android.widget.TextView;

import me.aflak.arduino.Arduino;

public class MainActivity extends Activity  {

    public static final String TAG = "FollowMeActivity";

    private static final int PREVIEW_WIDTH = 640;
    private static final int PREVIEW_HEIGHT = 480;

    private AutoFitDrawableView mAutoDrawable;
    private VisionPresenter mVisionPresenter;

    private Button mFollowButton;
    private Button mHomeButton;
    private TextView textReceivedMessage;

    private LinearLayout mButtonLayout;

    private boolean isFollowing;
    private boolean isNavigating;



    private interface NavReachedListner {
        public void onChange();
    }

    private NavReachedListner mNavComplete = new NavReachedListner() {
        @Override
        public void onChange() {
            if (mVisionPresenter.getState() == VisionPresenter.States.END_NAV) {
                mButtonLayout.setEnabled(true);
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_main);
            initView();
            initListener();
            isFollowing = false;

    }



    @Override
    protected void onResume() {
        super.onResume();
        int rotation = getWindowManager().getDefaultDisplay().getRotation();
        mAutoDrawable.setPreviewSizeAndRotation(PREVIEW_WIDTH, PREVIEW_HEIGHT, rotation);
        mAutoDrawable.setSurfaceTextureListenerForPerview(mSurfaceTextureListener);
        isFollowing = false;
    }

    @Override
    protected void onStop() {
        super.onStop();
        mVisionPresenter.stopPresenter();
        finish();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mVisionPresenter.stopPresenter();
        finish();
    }

    //@SuppressLint("WrongViewCast")
    private void initView() {
        mAutoDrawable = findViewById(R.id.drawableView);

        mFollowButton = findViewById(R.id.followButton);
        mHomeButton = findViewById(R.id.homeButton);
        //textReceivedMessage = findViewById(R.id.text_received_message);

        mButtonLayout = findViewById(R.id.buttonLayout);
    }




    private void initListener() {
        mFollowButton.setOnClickListener(followListener);
        mHomeButton.setOnClickListener(homeListener);


    }

    private TextureView.SurfaceTextureListener mSurfaceTextureListener = new TextureView.SurfaceTextureListener() {
        @Override
        public void onSurfaceTextureAvailable(SurfaceTexture surfaceTexture, int i, int i1) {
            mVisionPresenter = new VisionPresenter(mViewChangeInterface, mButtonLayout);
            mVisionPresenter.startPresenter();
        }

        @Override
        public void onSurfaceTextureSizeChanged(SurfaceTexture surfaceTexture, int i, int i1) {

        }

        @Override
        public boolean onSurfaceTextureDestroyed(SurfaceTexture surfaceTexture) {
            return false;
        }

        @Override
        public void onSurfaceTextureUpdated(SurfaceTexture surfaceTexture) {

        }

    };

    private ViewChangeInterface mViewChangeInterface = new ViewChangeInterface() {
        @Override
        public AutoFitDrawableView getAutoDrawable() {
            return mAutoDrawable;
        }
    };

    private View.OnClickListener followListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            if (isNavigating) mVisionPresenter.endNav();

            if (!isFollowing) {
                mVisionPresenter.beginFollow();
                isFollowing = true;


            }
            else  {
                mVisionPresenter.endFollow();
                isFollowing = false;
            }

            Log.d(TAG, "Following State: " + isFollowing);
        }
    };



    private View.OnClickListener homeListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {

//            mVisionPresenter.onArduinoMessage();


        }
    };



    // TESTING SEND FRA ARDUINO TIL LOOMO //
    public void onArduinoMessage(byte[] bytes) {
        String message = new String(bytes);
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                textReceivedMessage.setText("Received message: " + message);
            }
        });
    }




}


 */