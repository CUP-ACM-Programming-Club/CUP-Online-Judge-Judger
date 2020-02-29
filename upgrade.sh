#!/bin/bash
git fetch --all && git reset --hard origin/master && cmake ./ && make && chmod a+x judge_client && mv judge_client /usr/bin
