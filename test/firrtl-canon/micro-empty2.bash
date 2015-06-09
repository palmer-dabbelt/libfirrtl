#include "tempdir.bash"

cat >Top.firx <<"EOF"
<firrtl>
  <circuit>
    <module name="Top"/>
    <module name="Top2"/>
  </circuit>
  <module name="Top">
    <ports>
    </ports>
    <statements>
    </statements>
  </module>
  <module name="Top2">
    <ports>
    </ports>
    <statements>
    </statements>
  </module>
</firrtl>
EOF

#include "harness.bash"
