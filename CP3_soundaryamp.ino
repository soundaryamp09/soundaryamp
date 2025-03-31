#include <EloquentTinyML.h>
#include "cirrhosis_model.h"   // Your model header file
#include "x_train.h"           // Training data header file (x_train is defined here)
#include "x_test.h"            // Test data header file (x_test is defined here)

#define NUMBER_OF_INPUTS 13        // Number of features in your dataset (adjust if different)
#define NUMBER_OF_OUTPUTS 3        // Three output classes: Low Risk, Moderate Risk, High Risk
#define TENSOR_ARENA_SIZE 16 * 1024  // Memory allocation for the model

// Initialize the TinyML model with EloquentTinyML
Eloquent::TinyML::TfLite<NUMBER_OF_INPUTS, NUMBER_OF_OUTPUTS, TENSOR_ARENA_SIZE> ml;

void setup() {
  Serial.begin(115200);

  // Load the model
  if (!ml.begin(cirrhosis_model)) {  // Use the model array defined in cirrhosis_model.h
    Serial.println("Failed to load the model");
    while (1);  // Stay here if model loading fails
  }
  Serial.println("Model loaded successfully!");
}

void loop() {
  // Testing phase using x_test
  Serial.println("Testing with x_test data...");
  for (int i = 0; i < sizeof(x_test) / sizeof(x_test[0]); i += NUMBER_OF_INPUTS + 1) {
    // Prepare the input data from x_test array
    float input[NUMBER_OF_INPUTS];
    for (int j = 0; j < NUMBER_OF_INPUTS; j++) {
      input[j] = x_test[i + j];  // Access the features for this sample
    }

    // Get the actual class (last element in each x_test sample)
    int actualClass = static_cast<int>(x_test[i + NUMBER_OF_INPUTS]);

    // Prediction array
    float prediction[NUMBER_OF_OUTPUTS];
    ml.predict(input, prediction);

    // Determine the predicted class based on the highest confidence score
    int predictedClass = 0;
    float maxScore = prediction[0];
    for (int k = 1; k < NUMBER_OF_OUTPUTS; k++) {
      if (prediction[k] > maxScore) {
        maxScore = prediction[k];
        predictedClass = k;
      }
    }

    // Print the predicted and actual class results
    Serial.print("Predicted: ");
    Serial.print(predictedClass == 0 ? "Low Risk" : predictedClass == 1 ? "Moderate Risk" : "High Risk");
    Serial.print(" (Confidence: ");
    Serial.print(maxScore, 4);
    Serial.print("), Actual: ");
    Serial.println(actualClass == 0 ? "Low Risk" : actualClass == 1 ? "Moderate Risk" : "High Risk");

    delay(2000);  // Delay for readability between predictions
  }

  // Halt after one loop through all test data
  while (1);
}
