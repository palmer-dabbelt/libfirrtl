#include "tempdir.bash"

cat >Top.firx <<"EOF"
<firrtl>
  <circuit>
    <module name="Top"/>
  </circuit>
  <module name="Top">
    <ports>
    </ports>
    <statements>
    </statements>
  </module>
</firrtl>
EOF

#include "harness.bash"
