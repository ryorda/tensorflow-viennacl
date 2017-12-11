/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <stdio.h>

#include "tensorflow/contrib/lite/java/src/main/native/custom.h"

/// logging
#include <android/log.h>
#include <fstream>
#include <time.h>
/// end of logging

#include "tensorflow/contrib/android_renderscript_ops/jni/RScommon.h"
#include "tensorflow/contrib/android_renderscript_ops/jni/rsMatmul.h"

#include <vector>

static sp<RS> mRS = new RS();

JNIEXPORT void JNICALL
Java_org_tensorflow_lite_Custom_copyVectorTest(JNIEnv* env, jclass /*clazz*/) {
	
	if(!mRS->getContext()){
        mRS->init(kCachePath);
    }

	for (auto size : {2, 4, 8, 16, 32, 64, 128, 256, 512,1024, 2048, 4096}){

		std::vector<float> vec;

		for (float i = 0; i < size; i++)
			vec.push_back(255);

		// allocation time
		timespec start, finish;
		clock_gettime(CLOCK_MONOTONIC, &start);
		
		sp<const Element> e = Element::F32(mRS);

        sp<const Type> t = Type::create(mRS, e, size, 1, 0);

        sp<Allocation> alloc = Allocation::createTyped(mRS, t, RS_ALLOCATION_USAGE_SHARED | RS_ALLOCATION_USAGE_SCRIPT);


		clock_gettime(CLOCK_MONOTONIC, &finish);
		float delta_time = (finish.tv_sec - start.tv_sec) + ((float)(finish.tv_nsec - start.tv_nsec)/1000000000.0f);

		__android_log_print(ANDROID_LOG_INFO, "LOG_TEST", " CopyVector %d : allocation , consume time : %f sec", size, delta_time );


		// copy time

		clock_gettime(CLOCK_MONOTONIC, &start);

		alloc->copy1DRangeFrom(0, size, &vec[0]);

		clock_gettime(CLOCK_MONOTONIC, &finish);
		delta_time = (finish.tv_sec - start.tv_sec) + ((float)(finish.tv_nsec - start.tv_nsec)/1000000000.0f);

		__android_log_print(ANDROID_LOG_INFO, "LOG_TEST", " CopyVector %d : copy , consume time : %f sec", size, delta_time );
	}
}

JNIEXPORT void JNICALL
Java_org_tensorflow_lite_Custom_copyMatrixTest(JNIEnv* env, jclass /*clazz*/) {
	

	if(!mRS->getContext()){
        mRS->init(kCachePath);
    }

	for (int size : {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024}){

		float matrix[size][size];

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				matrix[i][j] = 255;
		
		// allocation time
		timespec start, finish;
		clock_gettime(CLOCK_MONOTONIC, &start);
		
		sp<const Element> e = Element::F32(mRS);

        sp<const Type> t = Type::create(mRS, e, size, size, 0);

        sp<Allocation> alloc = Allocation::createTyped(mRS, t, RS_ALLOCATION_USAGE_SHARED | RS_ALLOCATION_USAGE_SCRIPT);


		clock_gettime(CLOCK_MONOTONIC, &finish);
		float delta_time = (finish.tv_sec - start.tv_sec) + ((float)(finish.tv_nsec - start.tv_nsec)/1000000000.0f);

		__android_log_print(ANDROID_LOG_INFO, "LOG_TEST", " CopyMatrix 2D %d : allocation , consume time : %f sec", size, delta_time );


		// copy time

		clock_gettime(CLOCK_MONOTONIC, &start);

		alloc->copy2DRangeFrom(0, 0, size, size, matrix);

		clock_gettime(CLOCK_MONOTONIC, &finish);
		delta_time = (finish.tv_sec - start.tv_sec) + ((float)(finish.tv_nsec - start.tv_nsec)/1000000000.0f);

		__android_log_print(ANDROID_LOG_INFO, "LOG_TEST", " CopyMatrix 2D %d : copy , consume time : %f sec", size, delta_time );

	}  

	// flattened matrix
	if(!mRS->getContext()){
        mRS->init(kCachePath);
    }

	for (int size : {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024}){

		float matrix[size * size];

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				matrix[i * size + j] = 255;
		
		// allocation time
		timespec start, finish;
		clock_gettime(CLOCK_MONOTONIC, &start);
		
		sp<const Element> e = Element::F32(mRS);

        sp<const Type> t = Type::create(mRS, e, size * size, 1, 0);

        sp<Allocation> alloc = Allocation::createTyped(mRS, t, RS_ALLOCATION_USAGE_SHARED | RS_ALLOCATION_USAGE_SCRIPT);


		clock_gettime(CLOCK_MONOTONIC, &finish);
		float delta_time = (finish.tv_sec - start.tv_sec) + ((float)(finish.tv_nsec - start.tv_nsec)/1000000000.0f);

		__android_log_print(ANDROID_LOG_INFO, "LOG_TEST", " CopyMatrix 1D %d : allocation , consume time : %f sec", size, delta_time );


		// copy time

		clock_gettime(CLOCK_MONOTONIC, &start);

		alloc->copy1DRangeFrom(0, size * size, matrix);

		clock_gettime(CLOCK_MONOTONIC, &finish);
		delta_time = (finish.tv_sec - start.tv_sec) + ((float)(finish.tv_nsec - start.tv_nsec)/1000000000.0f);

		__android_log_print(ANDROID_LOG_INFO, "LOG_TEST", " CopyMatrix 1D %d : copy , consume time : %f sec", size, delta_time );

	} 
}

JNIEXPORT void JNICALL
Java_org_tensorflow_lite_Custom_multiplicationMatrixTest(JNIEnv* env, jclass /*clazz*/) {
	

	if(!mRS->getContext()){
        mRS->init(kCachePath);
    }

	for (int size : {2, 4, 8, 16, 32, 64, 128, 256, 512}){

		float matrix[size * size], matrix2[size * size], result[size * size];

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				matrix[i * size + j] = 255, matrix2[i * size + j] = 255;
		
		// allocation time
		timespec start, finish;
		clock_gettime(CLOCK_MONOTONIC, &start);

		androidrs::matmul::rsMatmul_sgemm(static_cast<void*>(const_cast<float*>(matrix)), 0,
			static_cast<void*>(const_cast<float*>(matrix2)), 0, 
			static_cast<void*>(const_cast<float*>(result)), size, size, size, 1, 0);
	

		clock_gettime(CLOCK_MONOTONIC, &finish);
		float delta_time = (finish.tv_sec - start.tv_sec) + ((float)(finish.tv_nsec - start.tv_nsec)/1000000000.0f);

		__android_log_print(ANDROID_LOG_INFO, "LOG_TEST", " multiplicationMatrix 2D %d : allocation , consume time : %f sec", size, delta_time );

	}  

}

JNIEXPORT void JNICALL
Java_org_tensorflow_lite_Custom_matrixVectorTest(JNIEnv* env, jclass /*clazz*/) {
	

	if(!mRS->getContext()){
        mRS->init(kCachePath);
    }

	for (int size : {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024}){

		float matrix[size * size], matrix2[size], result[size];

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				matrix[i * size + j] = 255, matrix2[i] = 255;
		
		// allocation time
		timespec start, finish;
		clock_gettime(CLOCK_MONOTONIC, &start);



		androidrs::matmul::rsMatmul_sgemv(
			static_cast<void*>(const_cast<float*>(matrix2)), 1, 
			static_cast<void*>(const_cast<float*>(matrix)), 
			static_cast<void*>(const_cast<float*>(result)), 1, 
            size, size, 1, 0, 0
            );

		// androidrs::matmul::rsMatmul_sgemm(static_cast<void*>(const_cast<float*>(matrix)), 0,
		// 	static_cast<void*>(const_cast<float*>(matrix2)), 0, 
		// 	static_cast<void*>(const_cast<float*>(result)), size, 1, size, 1, 0);


		clock_gettime(CLOCK_MONOTONIC, &finish);
		float delta_time = (finish.tv_sec - start.tv_sec) + ((float)(finish.tv_nsec - start.tv_nsec)/1000000000.0f);

		__android_log_print(ANDROID_LOG_INFO, "LOG_TEST", " MatrixVector 2D %d , consume time : %f sec", size, delta_time );

	}  

	// test cache
	for (int i = 0; i < 4	; i++){

		int size = 1024;


		float matrix[size * size], matrix2[size], result[size];

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				matrix[i * size + j] = 255, matrix2[i] = 255;
		
		// allocation time
		timespec start, finish;
		clock_gettime(CLOCK_MONOTONIC, &start);


		androidrs::matmul::rsMatmul_sgemv(
			static_cast<void*>(const_cast<float*>(matrix2)), 1, 
			static_cast<void*>(const_cast<float*>(matrix)), 
			static_cast<void*>(const_cast<float*>(result)), 1, 
            size, size, 1, 0, 1
            );

		// androidrs::matmul::rsMatmul_sgemm(static_cast<void*>(const_cast<float*>(matrix)), 0,
		// 	static_cast<void*>(const_cast<float*>(matrix2)), 0, 
		// 	static_cast<void*>(const_cast<float*>(result)), size, 1, size, 1, 0);


		clock_gettime(CLOCK_MONOTONIC, &finish);
		float delta_time = (finish.tv_sec - start.tv_sec) + ((float)(finish.tv_nsec - start.tv_nsec)/1000000000.0f);

		__android_log_print(ANDROID_LOG_INFO, "LOG_TEST", " MatrixVector 2D %d , consume time : %f sec", size, delta_time );

	}  

}

JNIEXPORT void JNICALL
Java_org_tensorflow_lite_Custom_matrixVectorNaiveTest(JNIEnv* env, jclass /*clazz*/) {
	
	for (int size : {4, 8, 16, 32, 64, 128, 256, 512, 1024}){

		// float matrix[size * size], matrix2[size], result[size];

		float *matrix = (float *)malloc(size * size * sizeof(float *));
		float *matrix2 = (float *)malloc(size * sizeof(float *));
		float *result = (float *)malloc(size * sizeof(float *));

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				matrix[i * size + j] = 1, matrix2[i] = 1;


		int result_stride = 1;
		int n_batch = 1;
		int m_cols = size;
		int m_rows = size;
		// allocation time
		timespec start, finish;
		clock_gettime(CLOCK_MONOTONIC, &start);


		float* result_in_batch = result;
		  for (int b = 0; b < n_batch; b++) {
		    const float* matrix_ptr = matrix;
		    for (int r = 0; r < m_rows; r++) {
		      const float* vector_in_batch = matrix2 + b * m_cols;
		      for (int c = 0; c < m_cols; c++) {
		        *result_in_batch += *matrix_ptr++ * *vector_in_batch++;
		      }
		      result_in_batch += result_stride;
		    }
		  }

		clock_gettime(CLOCK_MONOTONIC, &finish);
		float delta_time = (finish.tv_sec - start.tv_sec) + ((float)(finish.tv_nsec - start.tv_nsec)/1000000000.0f);

		bool isRight = true;
		for (int i = 0; i < size; i++){
			isRight = isRight && result[i] == size;
		}
		__android_log_print(ANDROID_LOG_INFO, "LOG_TEST", " NaiveMatrixVector 1D %d : %s , consume time : %f sec", size, (isRight ? "true" : "false"), delta_time );

	}


	return;
}
