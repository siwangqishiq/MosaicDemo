package com.example.panyi.mosaicdemo;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private ImageView mImage;
    private Button mBtn;

    private Bitmap mBitmap;

    private SeekBar mSeekBar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mImage = (ImageView)findViewById(R.id.img);
        mBtn = (Button)findViewById(R.id.btn);
        mSeekBar = (SeekBar)findViewById(R.id.seek_bar);

        mBtn.setText(NativeProcess.stringFromJNI());
        mBtn.setOnClickListener(this);

        mBitmap = openBitmapWithARGB(getResources() ,R.drawable.test4);
        mImage.setImageBitmap(mBitmap);

        mSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int radius, boolean b) {
                Bitmap out = Bitmap.createBitmap(mBitmap.getWidth() , mBitmap.getHeight() , Bitmap.Config.ARGB_8888);
                NativeProcess.mosaticFilter(mBitmap ,out , radius);
                mImage.setImageBitmap(out);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
    }

    public static Bitmap openBitmapWithARGB(Resources res , int id){
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inPreferredConfig = Bitmap.Config.ARGB_8888;
        //options.inPreferredConfig = Bitmap.Config.RGB_565;
        return BitmapFactory.decodeResource(res, id,options);
    }

    @Override
    public void onClick(View view) {
        Bitmap out = Bitmap.createBitmap(mBitmap.getWidth() , mBitmap.getHeight() , Bitmap.Config.ARGB_8888);

        NativeProcess.mosaticFilter(mBitmap ,out , 50);
        mImage.setImageBitmap(out);
    }


}//end class
