import speech_recognition as sr
import os
import sys
import webbrowser
from enum import Enum


class Modes(Enum):
    ON = "включись"
    OFF = "выключись"
    BLINKING = "мигай"
    RED = "красный"
    GREEN = "зеленый"
    BLUE = "синий"
    EXIT = "стоп"


mode = Modes.OFF


def talk(words):
    print(words)
    os.system("say " + words)


talk("Привет, чем я могу помочь вам?")

""" 
    Функция command() служит для отслеживания микрофона.
    Вызывая функцию мы будет слушать что скажет пользователь,
    при этом для прослушивания будет использован микрофон.
    Получение данные будут сконвертированы в строку и далее
    будет происходить их проверка.
"""


def command():
    # Создаем объект на основе библиотеки
    # speech_recognition и вызываем метод для определения данных
    r = sr.Recognizer()

    # Начинаем прослушивать микрофон и записываем данные в source
    with sr.Microphone() as source:
        # Просто вывод, чтобы мы знали когда говорить
        print("Говорите")
        # Устанавливаем паузу, чтобы прослушивание
        # началось лишь по прошествию 1 секунды
        r.pause_threshold = 0.6
        # используем adjust_for_ambient_noise для удаления
        # посторонних шумов из аудио дорожки
        r.adjust_for_ambient_noise(source, duration=1)
        # Полученные данные записываем в переменную audio
        # пока мы получили лишь mp3 звук
        audio = r.listen(source, phrase_time_limit=1)

    try:  # Обрабатываем все при помощи исключений
        """ 
        Распознаем данные из mp3 дорожки.
        Указываем что отслеживаемый язык русский.
        Благодаря lower() приводим все в нижний регистр.
        Теперь мы получили данные в формате строки,
        которые спокойно можем проверить в условиях
        """
        zadanie = r.recognize_google(audio, language="ru-RU").lower()
        # Просто отображаем текст что сказал пользователь
        print("Вы сказали: " + zadanie)
    # Если не смогли распознать текст, то будет вызвана эта ошибка
    except sr.UnknownValueError:
        # Здесь просто проговариваем слова "Я вас не поняла"
        # и вызываем снова функцию command() для
        # получения текста от пользователя
        talk("Я вас не поняла")
        zadanie = command()

    # В конце функции возвращаем текст задания
    # или же повторный вызов функции
    return zadanie


def turnBlinkingOn():
    mode = Modes.BLINKING
    # Timer1.attachInterrupt(timerIsr);


def turnBlinkingOff():
    mode = Modes.ON
    # Timer1.attachInterrupt(timerIsr);


def turnOn():
    mode = Modes.ON
    # digitalWrite(pinRed, LOW)
    # digitalWrite(pinBlue, LOW)
    # digitalWrite(pinGreen, LOW)
    # Timer1.detachInterrupt()


def turnOff():
    mode = Modes.OFF
    # digitalWrite(pinRed, LOW)
    # digitalWrite(pinBlue, LOW)
    # digitalWrite(pinGreen, LOW)
    # Timer1.detachInterrupt()


# Данная функция служит для проверки текста,
# что сказал пользователь (zadanie - текст от пользователя)

def makeSomething(zadanie):
    match zadanie:
        case Modes.ON.value:
            turnBlinkingOn()
        case Modes.OFF.value:
            turnOn()
        case Modes.BLINKING.value | Modes.BLUE.value | Modes.GREEN.value | Modes.RED.value:
            turnOff()
        case Modes.EXIT.value:
            sys.exit()


# Вызов функции для проверки текста будет
# осуществляться постоянно, поэтому здесь
# прописан бесконечный цикл while
while True:
    makeSomething(command())
