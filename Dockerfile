FROM quay.io/pypa/manylinux2010_x86_64

COPY . /io
WORKDIR /io

RUN /opt/python/cp310-cp310/bin/python setup.py sdist bdist_wheel
