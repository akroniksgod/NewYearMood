import speech_recognition as sr
import os
import sys
from enum import Enum


# Перечень допустимых режимов.
#   ON - включаем все лампочки.
#   OFF - выключаем все лампочки.
#   BLINKING - режим мигания.
#   RED - красная подсветка
#   GREEN - зелёная подсветка
#   BLUE - синяя подсветка
class Modes(Enum):
    ON = "включись"
    OFF = "выключись"
    BLINKING = "мигай"
    RED = "красный"
    GREEN = "зеленый"
    BLUE = "синий"
    EXIT = "стоп"


CURRENT_MODE = Modes.OFF


def talk(words):
    print(words)
    os.system("Говорите " + words)


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
        user_command = r.recognize_google(audio, language="ru-RU").lower()
        # Просто отображаем текст, что сказал пользователь
        print("Вы сказали: " + user_command)
    # Если не смогли распознать текст, то будет вызвана эта ошибка
    except sr.UnknownValueError:
        # Здесь просто проговариваем слова "Я вас не поняла"
        # и вызываем снова функцию command() для
        # получения текста от пользователя
        talk("Я вас не поняла")
        user_command = command()

    # В конце функции возвращаем текст задания
    # или же повторный вызов функции
    return user_command


def turn_blinking_on():
    CURRENT_MODE = Modes.BLINKING


def turn_on():
    CURRENT_MODE = Modes.ON


def turn_off():
    CURRENT_MODE = Modes.OFF


# Данная функция служит для проверки текста,
# что сказал пользователь (user_command - текст от пользователя)

def main_loop(user_command):
    if user_command == Modes.ON.value:
        turn_blinking_on()
        return

    if user_command == Modes.OFF.value:
        turn_on()
        return

    if user_command == Modes.BLINKING.value | user_command == Modes.BLUE.value | user_command == Modes.GREEN.value | user_command == Modes.RED.value:
        turn_off()
        return

    if user_command == Modes.EXIT.value:
        sys.exit()


# Вызов функции для проверки текста будет
# осуществляться постоянно, поэтому здесь
# прописан бесконечный цикл while
while True:
    main_loop(command())
