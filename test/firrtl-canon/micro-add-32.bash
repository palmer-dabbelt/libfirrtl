#include "tempdir.bash"

cat >Top.firx <<"EOF"
<firrtl>
  <circuit>
    <module name="Top"/>
  </circuit>
  <module name="Top">
    <ports>
      <port name="i">
        <uint width="32">
      </port>
      <port name="o">
        <uint width="32">
      </port>
    </ports>
    <statements>
      <statement>
        <destination>
          <reference name="o"/>
        </destination>
        <operation type=":="/>
        <source>
          <reference name="i"/>
        </source>
        <source>
          <reference name="i"/>
        </source>
      </statement>
    </statements>
  </module>
</firrtl>
EOF

#include "harness.bash"
