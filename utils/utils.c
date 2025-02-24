#include "utils.h"

state_t init_state(const char *ss[2], const char *mds[NB_PIECES]) {
  state_t st = {};
  st.shader = LoadShader(ss[0], ss[1]);
  for (size_t i = 0; i < NB_PIECES; ++i) {
    st.pieces[i] = LoadModel(mds[i]);
  }
  st.c_select[0] = UNDEF_COORD;
  st.c_select[1] = UNDEF_COORD;
  st.lights[0] = CreateLight(LIGHT_POINT, Vector3Zero(), Vector3Zero(), WHITE,
      st.shader);
  st.lights[1] = CreateLight(LIGHT_POINT, Vector3Zero(), Vector3Zero(), WHITE,
      st.shader);
  st.mk_screen = true;
  st.screens = nullptr;
  st.shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(st.shader,
      "viewPos");
  for (size_t k = 0; k < NB_PIECES; ++k) {
    st.pieces[k].materials[0].shader = st.shader;
  }
  return st;
}
