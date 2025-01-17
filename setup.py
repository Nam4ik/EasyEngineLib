from setuptools import setup, find_packages

def readme():
    with open('README.md', 'r') as f:
        return f.read()



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
    classifiers=[
        'Programming language :: C++, Python',
        'License :: OSI Appreoved :: MIT',
        'Operating System :: OS Independent'
        ],
    keywords='Graphics 2d',
    project_urls={
        'GitHub': 'https://GitHub.com/Nam4ik/EasyEngineLib'
        },
    python_requires='>=3.6'
    
        )
