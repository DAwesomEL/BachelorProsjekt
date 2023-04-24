package com.infintro.loomocart;

import android.util.Log;
import android.view.Surface;
import android.widget.LinearLayout;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Stack;

import com.segway.robot.algo.Pose2D;
import com.segway.robot.algo.dts.BaseControlCommand;
import com.segway.robot.algo.dts.DTSPerson;
import com.segway.robot.algo.dts.PersonTrackingProfile;
import com.segway.robot.algo.dts.PersonTrackingWithPlannerListener;
import com.segway.robot.algo.minicontroller.CheckPoint;
import com.segway.robot.algo.minicontroller.CheckPointStateListener;
import com.segway.robot.sdk.base.bind.ServiceBinder;
import com.segway.robot.sdk.locomotion.head.Angle;
import com.segway.robot.sdk.locomotion.sbv.Base;
import com.segway.robot.sdk.locomotion.head.Head;
import com.segway.robot.sdk.locomotion.sbv.StartVLSListener;
import com.segway.robot.sdk.vision.DTS;
import com.segway.robot.sdk.vision.Vision;
import com.segway.robot.sdk.voice.Speaker;
import com.segway.robot.sdk.voice.Recognizer;
import com.segway.robot.sdk.voice.VoiceException;
import com.segway.robot.sdk.voice.recognition.RecognitionResult;
import com.segway.robot.sdk.voice.recognition.WakeupListener;
import com.segway.robot.sdk.voice.recognition.WakeupResult;
import com.segway.robot.support.control.HeadPIDController;
import com.segway.robot.sdk.voice.recognition.RecognitionListener;
import com.segway.robot.sdk.voice.tts.TtsListener;
import com.segway.robot.sdk.voice.grammar.GrammarConstraint;





public class VisionPresenter {
    private final static String TAG = "VisionPresenter";

    public enum States {INIT_TRACK, END_TRACK, INIT_NAV, END_NAV}

    private final static int TIME_OUT = 10*1000;


    private ViewChangeInterface mViewInterface;
    private LinearLayout mButtonLayout;





    //service interfaces
    private Vision mVision;
    private Head mHead;
    private Base mBase;
    private HeadPIDController mHeadPID = new HeadPIDController();
    private Recognizer mRecognizer;
    private Speaker mSpeaker;

    //tracking variables
    private PersonTrackingProfile mPersonTracking;
    private DTSPerson[] mPersons;

    //booleans for checking of a given service is bound
    private boolean isHeadBind;
    private boolean isBaseBind;
    private boolean isVisionBind;
    private boolean isRecognizerBind;
    private boolean isSpeakerBind;



    private long startTime;

    private DTS mDTS;

    private States mState;

    //audio specific variables
    private ServiceBinder.BindStateListener mRecognitionBindStateListener;
    private ServiceBinder.BindStateListener mSpeakerBindStateListener;
    private TtsListener mTtsListener;

    //serial communication variable
    private SerialCommunicator mSerial;




    //signals for the trailer lights
    // "L" is left turn, "R" is right turn, "G" is go, "S" is stop
    private String[][] signals = {
            {"L", "G", "G", "S"},
    };




    /* Initialize the Vision Presenter */
    public VisionPresenter(ViewChangeInterface _ViewInterface, LinearLayout _ButtonLayout) {
        mViewInterface = _ViewInterface;
        mButtonLayout = _ButtonLayout;
        mSerial = new SerialCommunicator(); //create new serial instance
    }

    public void startPresenter() {
        mVision = Vision.getInstance();
        mHead = Head.getInstance();
        mBase = Base.getInstance();
        mRecognizer = Recognizer.getInstance();
        mSpeaker = Speaker.getInstance();

        setBindRecognitionService();

        mVision.bindService(LoomoCart.getContext(), mVisionBindStateListener);
        mHead.bindService(LoomoCart.getContext(), mHeadStateListener);
        mBase.bindService(LoomoCart.getContext(), mBaseStateListener);
        mRecognizer.bindService(LoomoCart.getContext(), mRecognitionBindStateListener);
        mSpeaker.bindService(LoomoCart.getContext(), mSpeakerBindStateListener);



        mPersonTracking = new PersonTrackingProfile(3, 1.0f);



        mSerial.serialBegin();  //begin serial communications

    }

    /* Stop the vision presenter */
    public void stopPresenter() {
        if (mDTS != null) {
            mDTS.stop();
            mDTS = null;
        }

        mVision.unbindService();
        mHead.unbindService();
        mBase.unbindService();
        mRecognizer.unbindService();

        mSerial.serialEnd();    //close serial
    }



    public States getState() {
        return mState;
    }

    private void resetHead() {
        mHead.setMode(Head.MODE_SMOOTH_TACKING);
        mHead.setWorldYaw(0);
        mHead.setWorldPitch(0.7f);
    }




    public void beginFollow() {
        if (mState == States.INIT_NAV) {
            speak("I cannot follow, I am currently navigating.", 0);
            return;
        }

        if (mState == States.INIT_TRACK) {
            speak("I am already following.", 0);
            return;
        }
        Log.d(TAG, "Beginning follow...");

        speak("I am following!", 50);
        mPersons = mDTS.detectPersons(3*1000*1000);
        startTime = System.currentTimeMillis();
        mState = States.INIT_TRACK;
        mDTS.startPlannerPersonTracking(mPersons[0], mPersonTracking, 60*1000*1000, mTrackingPlanner);
        //mSerial.sendBytes("1".getBytes());

        String cmd = "G";
        //mSerial.sendBytes(cmd.getBytes());
        mSerial.sendBytes("G".getBytes());



    }






    public void endFollow() {
        if (mState == States.INIT_TRACK) {
            Log.d(TAG, "Ending follow...");
            speak("I am not following anymore.", 50);
            mState = States.END_TRACK;
            mDTS.stopPlannerPersonTracking();
            mHeadPID.stop();
            mBase.clearCheckPointsAndStop();
            //mSerial.sendBytes("0".getBytes());
            String cmd = "S";
            mSerial.sendBytes(cmd.getBytes());



            resetHead();
            Log.d(TAG, "Follow stopped.");
        } else {
            speak("I am not currently following.", 0);
        }
    }


    public void beginNav() {




    }


    public void endNav() {
        if (mState == States.INIT_NAV) {
            Log.d(TAG, "Ending nav...");
            mBase.clearCheckPointsAndStop();
            mBase.stopVLS();
            //mBase.setControlMode(Base.CONTROL_MODE_RAW);
            //mBase.stop();
            mState = States.END_NAV;
            Log.d(TAG, "Nav stopped.");
            speak("I have arrived.", 0);
//            enableButtons();
        } else {
            speak("I am not currently navigating.", 0);
        }
    }

    /* Tracking Listeners */
    private PersonTrackingWithPlannerListener mTrackingPlanner = new PersonTrackingWithPlannerListener() {
        @Override
        public void onPersonTrackingWithPlannerResult(DTSPerson person, BaseControlCommand baseControlCommand) {
            if (person == null) {
                if (System.currentTimeMillis() - startTime > TIME_OUT) {
                    resetHead();
                }

                return;
            }


            startTime = System.currentTimeMillis();
            mHead.setMode(Head.MODE_ORIENTATION_LOCK);
            mHeadPID.updateTarget(person.getTheta(), person.getDrawingRect(), 480);

            AutoFitDrawableView autoFitDrawableView = mViewInterface.getAutoDrawable();
            autoFitDrawableView.drawRect(person.getDrawingRect());

            switch (baseControlCommand.getFollowState()) {
                case BaseControlCommand.State.NORMAL_FOLLOW:
                    mBase.setControlMode(Base.CONTROL_MODE_RAW);
                    mBase.setLinearVelocity(baseControlCommand.getLinearVelocity());
                    mBase.setAngularVelocity(baseControlCommand.getAngularVelocity());
                    break;
                case BaseControlCommand.State.HEAD_FOLLOW_BASE:
                    mBase.setControlMode(Base.CONTROL_MODE_FOLLOW_TARGET);
                    mBase.updateTarget(0, person.getTheta());
                    break;
                case BaseControlCommand.State.SENSOR_ERROR:
                    mBase.setControlMode(Base.CONTROL_MODE_RAW);
                    mBase.setLinearVelocity(0);
                    mBase.setAngularVelocity(0);
                    break;
            }
        }

        @Override
        public void onPersonTrackingWithPlannerError(int errorCode, String message) {
            Log.d("PersonTracking", message);
        }
    };





    /* Service Bind State Listeners */
    private ServiceBinder.BindStateListener mVisionBindStateListener = new ServiceBinder.BindStateListener() {
        @Override
        public void onBind() {
            isVisionBind = true;
            mDTS = mVision.getDTS();
            mDTS.setVideoSource(DTS.VideoSource.CAMERA);
            AutoFitDrawableView autoFitDrawableView = mViewInterface.getAutoDrawable();
            Surface surface = new Surface(autoFitDrawableView.getPreview().getSurfaceTexture());
            mDTS.setPreviewDisplay(surface);
            mDTS.start();
        }

        @Override
        public void onUnbind(String reason) {
            isVisionBind = false;
        }
    };

    private ServiceBinder.BindStateListener mBaseStateListener = new ServiceBinder.BindStateListener() {
        @Override
        public void onBind() {
            isBaseBind = true;
        }

        @Override
        public void onUnbind(String reason) {
            isBaseBind = false;
        }
    };

    private ServiceBinder.BindStateListener mHeadStateListener = new ServiceBinder.BindStateListener() {
        @Override
        public void onBind() {
            isHeadBind = true;
            resetHead();
            mHeadPID.init(HeadControlHandlerImpl);
            mHeadPID.setHeadFollowFactor(1.0f);
        }

        @Override
        public void onUnbind(String reason) {
            isHeadBind = false;
        }
    };

    /* Head PID Controller Handler */
    private HeadPIDController.HeadControlHandler HeadControlHandlerImpl = new HeadPIDController.HeadControlHandler() {
        @Override
        public float getJointYaw() {
            Angle angle = mHead.getHeadJointYaw();
            if (angle == null) {
                return 0;
            }
            return angle.getAngle();
        }

        @Override
        public float getJointPitch() {
            Angle angle = mHead.getHeadJointPitch();
            if (angle == null) {
                return 0;
            }
            return angle.getAngle();
        }

        @Override
        public void setYawAngularVelocity(float velocity) {
            mHead.setYawAngularVelocity(velocity);
        }

        @Override
        public void setPitchAngularVelocity(float velocity) {
            mHead.setPitchAngularVelocity(velocity);
        }
    };


    /*
    * All Voice Recognition Definitions
    * MÅ VÆRE FORELØPIG PROGRAMMET KRASJER VISS DENNE FJERNES!!!!!!
    *
    * */

    //voice recognition listener
    private RecognitionListener mRecognitionListener = new RecognitionListener() {
        @Override
        public void onRecognitionStart() {
            Log.d(TAG, "Starting voice recognition...");
        }

        @Override
        public boolean onRecognitionResult(RecognitionResult recognitionResult) {
            Log.d(TAG, String.valueOf(recognitionResult.getConfidence()));
            Log.d(TAG, recognitionResult.toString());

            String result = recognitionResult.getRecognitionResult();

            Log.d(TAG, "SPEECH RECOGNITION RESULT: " + result);

            return false;
        }

        @Override
        public boolean onRecognitionError(String error) {
            return false;
        }
    };



    private void setBindRecognitionService() {
        Log.d(TAG, "SETTING UP BIND STATE LISTENERS");

        //service binder listener service
        mRecognitionBindStateListener = new ServiceBinder.BindStateListener() {
            @Override
            public void onBind() {


                isRecognizerBind = true;
            }

            @Override
            public void onUnbind(String reason) {
                isRecognizerBind = false;
            }
        };

        mSpeakerBindStateListener = new ServiceBinder.BindStateListener() {
            @Override
            public void onBind() {
                Log.d(TAG, "STARTING SPEAKER BIND LISTENER");

                isSpeakerBind = true;

               //tell the user Loomo's speech is now working
                speak("I am alive.", 50);
            }

            @Override
            public void onUnbind(String reason) {

            }
        };



        mTtsListener = new TtsListener() {
            @Override
            public void onSpeechStarted(String s) {
                //s is speech content, callback this method when speech is starting.
                Log.d(TAG, "onSpeechStarted() called with: s = [" + s + "]");
            }

            @Override
            public void onSpeechFinished(String s) {
                //s is speech content, callback this method when speech is finish.
                Log.d(TAG, "onSpeechFinished() called with: s = [" + s + "]");
            }

            @Override
            public void onSpeechError(String s, String s1) {
                //s is speech content, callback this method when speech occurs error.
                Log.d(TAG, "onSpeechError() called with: s = [" + s + "], s1 = [" + s1 + "]");
            }
        };
    }



    //making speech easier and also a class call
    private void speak(String message, int volume) {
        try {
            mSpeaker.setVolume(volume);
            mSpeaker.speak(message, mTtsListener);
        } catch (VoiceException e) {
            Log.e(TAG, "SPEAKING EXCEPTION: ", e);
        }
    }




}
