FROM 0xfeedc0de64/qt-build

ADD . /tmp/WifiLamp

RUN mkdir -p /tmp/build_WifiLamp \
 && qmake /tmp/WifiLamp -o /tmp/build_WifiLamp/Makefile -config release \
 && make -C /tmp/build_WifiLamp -j8 \
 && make -C /tmp/build_WifiLamp install



FROM 0xfeedc0de64/qt-runtime

COPY --from=0 /tmp/build_WifiLamp/WifiLamp /root/

EXPOSE 1234/tcp
EXPOSE 8080/tcp

CMD /root/WifiLamp
