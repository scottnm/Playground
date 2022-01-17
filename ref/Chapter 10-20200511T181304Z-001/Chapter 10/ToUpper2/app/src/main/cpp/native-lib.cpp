#include <jni.h>
#include <string>

extern "C" int mytoupper( const char * input, char * output);

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_toupper2_MainActivity_toupperJNI(
        JNIEnv* env,
        jobject /* this */,
        jstring input) {
    char upperStr[255];

    mytoupper(env->GetStringUTFChars(input, NULL), upperStr);
    return env->NewStringUTF(upperStr);
}

/* int mytoupper( const char * input, char * output)
{
    int i = 0;

    while( input[i] != NULL ) {
        if (input[i] >= 'a' && input[i] <= 'z') {
            output[i] = input[i] - ('a' - 'A');
        } else
        {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = NULL;
    return i;
} */
