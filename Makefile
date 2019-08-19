all: clean build fileXio

helloWorld-clean:
	$(MAKE) -C HelloWorld clean
helloWorld-build:
	$(MAKE) -C helloWorld

printFloatValue-clean:
	$(MAKE) -C PrintFloatValue clean
printFloatValue-build:
	$(MAKE) -C PrintFloatValue

fileXio-clean:
	$(MAKE) -C FileXio clean
fileXio-build:
	$(MAKE) -C FileXio


clean: helloWorld-clean printFloatValue-clean fileXio-clean

build: helloWorld-build printFloatValue-build fileXio-build