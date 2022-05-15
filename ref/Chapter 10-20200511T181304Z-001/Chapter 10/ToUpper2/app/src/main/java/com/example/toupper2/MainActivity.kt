package com.example.toupper2

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.TextView
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        //sample_text.text = stringFromJNI()
    }

    /** Called when the user taps the Send button */
    fun convertMessage(view: View) {
        // Do something in response to button
        val editText = findViewById<EditText>(R.id.enterText)
        val message = toupperJNI(editText.text.toString())
        val textView = findViewById<TextView>(R.id.convertedText).apply {
            text = message
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun toupperJNI(input: String): String

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
