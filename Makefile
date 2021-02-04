docker-build:
	docker-compose \
	-p cpp-system-monitor \
	-f dcomposes/docker-compose.yml \
	build

basic-monitor:
	docker-compose \
	-p cpp-system-monitor \
	-f dcomposes/docker-compose.basic.yml \
	run cpp

system-monitor:
	docker-compose \
	-p cpp-system-monitor \
	-f dcomposes/docker-compose.yml \
	run cpp

compile-monitor:
	docker-compose \
	-p cpp-system-monitor \
	-f dcomposes/docker-compose.justcompile.yml \
	run cpp
