from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext
import os

def readme():
    with open('README.md', 'r') as f:
        return f.read()

def readme():
    with open('README.md', 'r') as f:
        return f.read()

module = Extension(
    'engine._engine',
    sources=[
        'engine/engine.c'
    ],
    libraries=[
        'SDL2', 'SDL2_image', 'pthread', 'portaudio', 'json-c'
    ],
    include_dirs=[
        '/usr/include/SDL2',  # Путь к заголовочным файлам SDL2
        '/usr/include/json-c'  # Путь к заголовочным файлам json-c
    ],
    library_dirs=[
        '/usr/lib'  # Путь к библиотекам
    ]
)



setup(
    name='EasyEngine',
    version='0.1',
    author='Namilsky',
    author_email='alive6863@gmail.com',
    description='This a simple SDL2 based 2d UI engine. Used for creating windows without borders. Can render jpg, png, gif and simple grafics on screen like python-turtle. Can be used like C/CPP header or python lib. Suppport async work',
    long_description=readme(),
    long_description_content_type='text/markdown',
    url='https://t.me/ArcaneDevStudio',
    #    url1='https://github.com/Nam4ik/',
    packages=find_packages(),
    install_requires=[''], #SDL2, and posix libs 
    #    classifiers=[
    #    'Programming language :: C++, Python'
    #    ],
    keywords='Graphics 2d',
    project_urls={
        'GitHub': 'https://GitHub.com/Nam4ik/EasyEngineLib'
        },
    python_requires='>=3.6'
    
        )
