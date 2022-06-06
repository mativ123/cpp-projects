import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

from imageai.Classification import ImageClassification
prediction = ImageClassification()

prediction.setModelTypeAsResNet50()
prediction.setModelPath("resnet50_imagenet_tf.2.0.h5")
prediction.loadModel()

predictions, probabilities = prediction.classifyImage("obelLectio.jpg", result_count=1)
for eachPrediction, eachProbability in zip(predictions, probabilities):
    print(eachPrediction , " : " , eachProbability)
