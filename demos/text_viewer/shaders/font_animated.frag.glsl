/*! 
 * \file font_animated.frag.glsl
 * \brief file font_animated.frag.glsl
 * 
 * Copyright 2013 by Nomovok Ltd.
 * 
 * Contact: info@nomovok.com
 * 
 * This Source Code Form is subject to the
 * terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with
 * this file, You can obtain one at
 * http://mozilla.org/MPL/2.0/.
 * 
 * \author Kevin Rogovin <kevin.rogovin@nomovok.com>
 * 
 */


shader_in mediump vec4 tex_color;


uniform mediump float animation_fx_interpol;
void
shader_main(void)
{
  mediump float d;


#ifdef NO_AA
  d=wrath_glyph_is_covered()*tex_color.a*(1.0-animation_fx_interpol);
#else
  d=wrath_glyph_compute_coverage()*tex_color.a*(1.0-animation_fx_interpol);
#endif

#if defined(WRATH_IS_OPAQUE_PASS)
  //if texel is too translucent, then the opaque pass discards the texel
  if(d<float(WRATH_TRANSLUCENT_THRESHOLD))
    discard;
#elif defined(WRATH_IS_TRANSLUCENT_PASS)
  if(d>=float(WRATH_TRANSLUCENT_THRESHOLD))
    d=0.0;
#endif

  wrath_FragColor=vec4(tex_color.xyz*d, d);

}
