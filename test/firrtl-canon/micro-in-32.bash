#include "tempdir.bash"

cat >Top.firx <<"EOF"
<firrtl>
  <circuit>
    <module name="Top"/>
  </circuit>
  <module name="Top">
    <ports>
      <input name="io_in">
        <uint width="32"/>
      </input>
    </ports>
    <statements>
    </statements>
  </module>
</firrtl>
EOF

#include "harness.bash"
