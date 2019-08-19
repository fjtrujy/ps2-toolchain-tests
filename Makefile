all: clean build

helloWorld-clean:
	$(MAKE) -C HelloWorld clean
helloWorld-build:
	$(MAKE) -C helloWorld

printFloatValue-clean:
	$(MAKE) -C PrintFloatValue clean
printFloatValue-build:
	$(MAKE) -C PrintFloatValue


clean: helloWorld-clean printFloatValue-clean

build: helloWorld-build printFloatValue-build