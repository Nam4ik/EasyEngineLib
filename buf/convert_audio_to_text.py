import speech_recognition as sr
import json

def transcribe_audio(audio_file_path):
    recognizer = sr.Recognizer()
    with sr.AudioFile(audio_file_path) as source:
        audio_data = recognizer.record(source)
        try:
            text = recognizer.recognize_google(audio_data, language="ru-RU")
            return text
        except sr.UnknownValueError:
            print("Google Speech Recognition could not understand audio")
            return None
        except sr.RequestError as e:
            print(f"Could not request results from Google Speech Recognition service; {e}")
            return None

if __name__ == "__main__":
    audio_file_path = "recording.wav"
    text = transcribe_audio(audio_file_path)
    if text:
        answer = {"prompt": text}
        with open("answer.json", "w") as f:
            json.dump(answer, f)
        print(f"Transcribed text: {text}")
    else:
        print("Failed to transcribe audio")
