docker-build:
	docker-compose \
	-p cpp-system-monitor \
	-f dcomposes/docker-compose.yml \
	build

basic-monitor:
	xhost + 127.0.0.1
	docker-compose \
	-p cpp-system-monitor \
	-f dcomposes/docker-compose.basic.yml \
	run cpp

system-monitor:
	xhost + 127.0.0.1
	docker-compose \
	-p cpp-system-monitor \
	-f dcomposes/docker-compose.yml \
	run cpp

tests:
	xhost + 127.0.0.1
	docker-compose \
	-p cpp-system-monitor \
	-f dcomposes/docker-compose.test.yml \
	up
