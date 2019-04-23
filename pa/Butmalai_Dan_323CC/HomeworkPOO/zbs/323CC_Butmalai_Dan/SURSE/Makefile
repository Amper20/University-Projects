build:
	javac -g main/Test.java
run: build
	java main.Test > output.txt
run_with_params: build
	java main.Test ./main/inputs/test03/customers.txt ./main/inputs/test03/store.txt  ./main/inputs/test03/events.txt ./main/inputs/test03/result.txt
clean:
	find . -name *.class -type f -delete
