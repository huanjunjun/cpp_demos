all: test1
	echo "hello huanjun"
test1 : test2
	echo "huanjun test1"
test2:test3
	echo "huanjun test3"
test3: 
	echo "huanjun test4"
