from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext
import os
#import pybind11 

sip_file = 'binding.sip'

Engine_Module = Extension(
    'EzEngine',
    sources=['src/engine.cpp'],  
    libraries=['SDL2', 'SDL2_image', 'portaudio', 'cjson', 'pthread'], 
    include_dirs=['/usr/include/']  
)


def readmerru():
    with open('READMERU.md', 'r') as f:
        return f.read()

def readmeen():
    with open('README.md', 'r') as f:
        return f.read()

# module = Extension(
#     'engine._engine',
#     sources=[
#         'engine/engine.cpp'
#     ],
#     libraries=[
#         'SDL2', 'SDL2_image', 'pthread', 'portaudio', 'json-c'
#     ],
#     include_dirs=[
#         '/usr/include/'  # Путь к заголовочным файлам SDL2
#         # Путь к заголовочным файлам json-c
#     ],
#     library_dirs=[
#             '/usr/lib'  # Путь к библиотекам
#         ]
#     )


setup(
    name='EasyEngine',
    version='0.2.1',
    author='Namilsky',
    author_email='alive6863@gmail.com',
    description='This a simple SDL2 based 2d UI engine. Used for creating windows without borders. Can render jpg, png, gif and simple grafics on screen like python-turtle. Can be used like C/CPP header or python lib. Suppport async work',
    long_description=readmeen(),
    long_description_content_type='text/markdown',
    url='https://t.me/ArcaneDevStudio',
    #    url1='https://github.com/Nam4ik/',
    
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: GPU',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Environment :: X11 Applications',
        'Environment :: Win32 (MS Windows)',
        'Natural Language :: English',
        'Natural Language :: Russian',
        'Programming language :: Python :: 3',
        'Programming language :: C++ :: GCC ',
        'Programming language :: C :: GCC ',
        'Operating System :: OS  Independent',

        ],

    packages=find_packages(),
    install_requires=[''], #SDL2, and posix libs 
    #    classifiers=[
    #    'Programming language :: C++, Python'
    #    ],
    keywords='Graphics 2d',
    project_urls={
        'GitHub': 'https://GitHub.com/Nam4ik/EasyEngineLib'
        },
    python_requires='>=3.6',
    zip_safe=True,
    ext_modules=[Engine_Module]
    
        )
        
