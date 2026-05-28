#include <idc.idc>

static in_mapped_memory(ea)
{
  return get_segm_start(ea) != BADADDR;
}

static set_name_safe(ea, name)
{
  set_name(ea, name, SN_FORCE);
}

static clear_name_safe(ea)
{
  // Remove existing user label at this address, if any.
  set_name(ea, "", SN_FORCE);
}

static force_data_word(ea)
{
  // On C31 in IDA, one address unit is a 32-bit wide byte.
  del_items(ea, DELIT_SIMPLE, 1);
  create_byte(ea);
}

static force_code_word(ea)
{
  del_items(ea, DELIT_SIMPLE, 1);
  create_insn(ea);
}

static bswap32(v)
{
  auto b0, b1, b2, b3;
  b0 = (v >> 24) & 0xFF;
  b1 = (v >> 16) & 0xFF;
  b2 = (v >> 8) & 0xFF;
  b3 = v & 0xFF;
  return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
}

static maybe_make_func(ea)
{
  if (!in_mapped_memory(ea))
    return 0;
  if (get_func_attr(ea, FUNCATTR_START) != BADADDR)
    return 1;
  return add_func(ea, BADADDR);
}

static vec_name(idx)
{
  if (idx == 0x00) return "vec_reset__c_int00";
  if (idx == 0x01) return "vec_int0_display";
  if (idx == 0x02) return "vec_int1_tv30";
  if (idx == 0x03) return "vec_int2_comm";
  if (idx == 0x04) return "vec_int3_atod";
  if (idx == 0x05) return "vec_xint0";
  if (idx == 0x06) return "vec_rint0";
  if (idx == 0x07) return "vec_xint1";
  if (idx == 0x08) return "vec_rint1";
  if (idx == 0x09) return "vec_tint0";
  if (idx == 0x0A) return "vec_tint1";
  if (idx == 0x0B) return "vec_dint";
  return form("vec_%02X", idx);
}

static main()
{
  auto named, marked_data, code_words, vec_named, vec_funcs, cleared, dp_set;
  auto ea, idx, vec_ea, target, vname, ptr_name, dp_reg;
  named = 0;
  marked_data = 0;
  code_words = 0;
  vec_named = 0;
  vec_funcs = 0;
  cleared = 0;
  dp_set = 0;

  // 0) Clear existing names in target section so reruns are clean.
  for (ea = 0x00000000; ea <= 0x0001EFC0; ea = ea + 1)
  {
    if (in_mapped_memory(ea))
    {
      clear_name_safe(ea);
      cleared = cleared + 1;
    }
  }

  // 1) Force main mapped section to code first. (disabled)
  // for (ea = 0x00000000; ea <= 0x0001EFC0; ea = ea + 1)
  // {
  //   if (in_mapped_memory(ea))
  //   {
  //     force_code_word(ea);
  //     code_words = code_words + 1;
  //   }
  // }

  // 2) Hardcode vector entries and vector targets.
  for (idx = 0; idx < 0x40; idx = idx + 1)
  {
    vec_ea = idx;
    if (!in_mapped_memory(vec_ea))
      continue;
    force_data_word(vec_ea);
    target = bswap32(get_wide_byte(vec_ea));
    vname = vec_name(idx);
    set_name_safe(vec_ea, vname);
    vec_named = vec_named + 1;
    ptr_name = form("%s_target_%08X", vname, target);
    if (in_mapped_memory(target))
    {
      set_name_safe(target, ptr_name);
      if (maybe_make_func(target))
        vec_funcs = vec_funcs + 1;
    }
  }
  if (in_mapped_memory(0x00000000))
    maybe_make_func(bswap32(get_wide_byte(0x00000000)));

  // 3) Apply DP=0 assumptions at SETDP-derived points.
  dp_reg = str2reg("dp");
  if (dp_reg != -1)
  {
    if (in_mapped_memory(0x0000006A)) {
      split_sreg_range(0x0000006A, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000006E)) {
      split_sreg_range(0x0000006E, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000007F)) {
      split_sreg_range(0x0000007F, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00000082)) {
      split_sreg_range(0x00000082, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00000085)) {
      split_sreg_range(0x00000085, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00000088)) {
      split_sreg_range(0x00000088, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000008E)) {
      split_sreg_range(0x0000008E, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00000092)) {
      split_sreg_range(0x00000092, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000116E)) {
      split_sreg_range(0x0000116E, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000046AB)) {
      split_sreg_range(0x000046AB, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000049AD)) {
      split_sreg_range(0x000049AD, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004B0F)) {
      split_sreg_range(0x00004B0F, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004B27)) {
      split_sreg_range(0x00004B27, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004B54)) {
      split_sreg_range(0x00004B54, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004B68)) {
      split_sreg_range(0x00004B68, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004BA9)) {
      split_sreg_range(0x00004BA9, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004BB7)) {
      split_sreg_range(0x00004BB7, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004C14)) {
      split_sreg_range(0x00004C14, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004D27)) {
      split_sreg_range(0x00004D27, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004D42)) {
      split_sreg_range(0x00004D42, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004D57)) {
      split_sreg_range(0x00004D57, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004D85)) {
      split_sreg_range(0x00004D85, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004E79)) {
      split_sreg_range(0x00004E79, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004E82)) {
      split_sreg_range(0x00004E82, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004EAE)) {
      split_sreg_range(0x00004EAE, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004EB3)) {
      split_sreg_range(0x00004EB3, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004EBC)) {
      split_sreg_range(0x00004EBC, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004F08)) {
      split_sreg_range(0x00004F08, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004F1B)) {
      split_sreg_range(0x00004F1B, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004F4C)) {
      split_sreg_range(0x00004F4C, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004F7B)) {
      split_sreg_range(0x00004F7B, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00004F8A)) {
      split_sreg_range(0x00004F8A, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00006123)) {
      split_sreg_range(0x00006123, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00006306)) {
      split_sreg_range(0x00006306, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00006351)) {
      split_sreg_range(0x00006351, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00006358)) {
      split_sreg_range(0x00006358, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000643A)) {
      split_sreg_range(0x0000643A, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000645B)) {
      split_sreg_range(0x0000645B, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000696F)) {
      split_sreg_range(0x0000696F, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00007091)) {
      split_sreg_range(0x00007091, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000073E8)) {
      split_sreg_range(0x000073E8, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00007F5C)) {
      split_sreg_range(0x00007F5C, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00007F9A)) {
      split_sreg_range(0x00007F9A, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00007FA8)) {
      split_sreg_range(0x00007FA8, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000080AE)) {
      split_sreg_range(0x000080AE, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000080C5)) {
      split_sreg_range(0x000080C5, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000080FA)) {
      split_sreg_range(0x000080FA, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000081F2)) {
      split_sreg_range(0x000081F2, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000820E)) {
      split_sreg_range(0x0000820E, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000822E)) {
      split_sreg_range(0x0000822E, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000826C)) {
      split_sreg_range(0x0000826C, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00008270)) {
      split_sreg_range(0x00008270, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00008274)) {
      split_sreg_range(0x00008274, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000082A6)) {
      split_sreg_range(0x000082A6, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00008303)) {
      split_sreg_range(0x00008303, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00008322)) {
      split_sreg_range(0x00008322, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00008330)) {
      split_sreg_range(0x00008330, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000834E)) {
      split_sreg_range(0x0000834E, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00008E7F)) {
      split_sreg_range(0x00008E7F, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00008E93)) {
      split_sreg_range(0x00008E93, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00008EC6)) {
      split_sreg_range(0x00008EC6, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00009163)) {
      split_sreg_range(0x00009163, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000923B)) {
      split_sreg_range(0x0000923B, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00009268)) {
      split_sreg_range(0x00009268, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00009279)) {
      split_sreg_range(0x00009279, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000929F)) {
      split_sreg_range(0x0000929F, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000092CD)) {
      split_sreg_range(0x000092CD, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000092D2)) {
      split_sreg_range(0x000092D2, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000092D9)) {
      split_sreg_range(0x000092D9, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x000092DD)) {
      split_sreg_range(0x000092DD, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00009339)) {
      split_sreg_range(0x00009339, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00009356)) {
      split_sreg_range(0x00009356, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x00009402)) {
      split_sreg_range(0x00009402, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000A30B)) {
      split_sreg_range(0x0000A30B, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000A319)) {
      split_sreg_range(0x0000A319, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000A855)) {
      split_sreg_range(0x0000A855, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000A85B)) {
      split_sreg_range(0x0000A85B, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000A865)) {
      split_sreg_range(0x0000A865, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000A8A1)) {
      split_sreg_range(0x0000A8A1, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000AB5A)) {
      split_sreg_range(0x0000AB5A, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
    if (in_mapped_memory(0x0000AF11)) {
      split_sreg_range(0x0000AF11, dp_reg, 0, SR_user);
      dp_set = dp_set + 1;
    }
  }

  // 5) Labels from spider output.
  if (in_mapped_memory(0x00000000)) {
    force_data_word(0x00000000);
    marked_data = marked_data + 1;
    set_name_safe(0x00000000, "COMP_DECODE_STACKI");
    set_cmt(0x00000000, "alts: CUSA_CRT_REG_SETUP_STRI, INFIN_HIGHVAL, PALL_PALLISTI, PALL_PALRAMI, PALL_PTTRAMI, PALL_RAWLOCSI", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00000040)) {
    force_data_word(0x00000040);
    marked_data = marked_data + 1;
    set_name_safe(0x00000040, "OACTIVEI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000042)) {
    force_data_word(0x00000042);
    marked_data = marked_data + 1;
    set_name_safe(0x00000042, "OACTIVE_PRIORITYI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000043)) {
    force_data_word(0x00000043);
    marked_data = marked_data + 1;
    set_name_safe(0x00000043, "DIRQ_OLOW_PRIORITYI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000044)) {
    force_data_word(0x00000044);
    marked_data = marked_data + 1;
    set_name_safe(0x00000044, "DIRQ_OHIGH_PRIORITYI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000045)) {
    force_data_word(0x00000045);
    marked_data = marked_data + 1;
    set_name_safe(0x00000045, "CAMERAPOSI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000047)) {
    force_data_word(0x00000047);
    marked_data = marked_data + 1;
    set_name_safe(0x00000047, "CAMERAMATRIXI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000004B)) {
    force_data_word(0x0000004B);
    marked_data = marked_data + 1;
    set_name_safe(0x0000004B, "transmatrixI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000004C)) {
    force_data_word(0x0000004C);
    marked_data = marked_data + 1;
    set_name_safe(0x0000004C, "transvectorYI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000004D)) {
    force_data_word(0x0000004D);
    marked_data = marked_data + 1;
    set_name_safe(0x0000004D, "POSTERMATI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000004E)) {
    force_data_word(0x0000004E);
    marked_data = marked_data + 1;
    set_name_safe(0x0000004E, "DIRQ_POSTERMAT2DI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000004F)) {
    force_data_word(0x0000004F);
    marked_data = marked_data + 1;
    set_name_safe(0x0000004F, "BLOWLISTI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000050)) {
    force_data_word(0x00000050);
    marked_data = marked_data + 1;
    set_name_safe(0x00000050, "tmpmatI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000051)) {
    force_data_word(0x00000051);
    marked_data = marked_data + 1;
    set_name_safe(0x00000051, "tmpmatY");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000052)) {
    force_data_word(0x00000052);
    marked_data = marked_data + 1;
    set_name_safe(0x00000052, "INVTABI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000053)) {
    force_data_word(0x00000053);
    marked_data = marked_data + 1;
    set_name_safe(0x00000053, "SCRNHXI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000054)) {
    force_data_word(0x00000054);
    marked_data = marked_data + 1;
    set_name_safe(0x00000054, "SCRNHYI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000055)) {
    force_data_word(0x00000055);
    marked_data = marked_data + 1;
    set_name_safe(0x00000055, "HIGH_CLIP_LEV8");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000061)) {
    force_data_word(0x00000061);
    marked_data = marked_data + 1;
    set_name_safe(0x00000061, "DIRQ_VECTORAYI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000062)) {
    force_data_word(0x00000062);
    marked_data = marked_data + 1;
    set_name_safe(0x00000062, "_PALLISTI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000064)) {
    set_name_safe(0x00000064, "DIRQ");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000091)) {
    set_name_safe(0x00000091, "DIRQ_NEXTOBJ");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000097)) {
    set_name_safe(0x00000097, "DIRQ_NOBREAK_CONTINUE");
    named = named + 1;
  }
  if (in_mapped_memory(0x000000A3)) {
    set_name_safe(0x000000A3, "DIRQ_TRU_UNIV");
    named = named + 1;
  }
  if (in_mapped_memory(0x000000A6)) {
    set_name_safe(0x000000A6, "DIRQ_TRANS_RET");
    named = named + 1;
  }
  if (in_mapped_memory(0x000000BA)) {
    set_name_safe(0x000000BA, "DIRQ_TRIVIAL_REJECTION");
    named = named + 1;
  }
  if (in_mapped_memory(0x000000C5)) {
    set_name_safe(0x000000C5, "DIRQ_NO_DEGRADE");
    named = named + 1;
  }
  if (in_mapped_memory(0x000000F6)) {
    set_name_safe(0x000000F6, "DIRQ_ROT3D");
    named = named + 1;
  }
  if (in_mapped_memory(0x000000FD)) {
    set_name_safe(0x000000FD, "DIRQ_CHECKTHEREG");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000112)) {
    set_name_safe(0x00000112, "DIRQ_IDENTITY_ROTATION");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000115)) {
    set_name_safe(0x00000115, "DIRQ_SELF_ORIENTING");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000124)) {
    set_name_safe(0x00000124, "DIRQ_INLP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000128)) {
    set_name_safe(0x00000128, "DIRQ_VECTORTRANSFULL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000129)) {
    set_name_safe(0x00000129, "DIRQ_VT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000140)) {
    set_name_safe(0x00000140, "DIRQ_VTL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000162)) {
    set_name_safe(0x00000162, "DIRQ_EOVCTR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000163)) {
    set_name_safe(0x00000163, "DIRQ_POLYPOLY_ENTER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000166)) {
    set_name_safe(0x00000166, "DIRQ_DISPLAY");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000169)) {
    set_name_safe(0x00000169, "DIRQ_TRANS2D");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000175)) {
    set_name_safe(0x00000175, "DIRQ_REGULAR1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000188)) {
    set_name_safe(0x00000188, "DIRQ_VT2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000001A0)) {
    set_name_safe(0x000001A0, "DIRQ_VTL2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000001BA)) {
    set_name_safe(0x000001BA, "DIRQ_EOVCTR2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000001BC)) {
    set_name_safe(0x000001BC, "DIRQ_DYNAMIC_OBJECT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000001CE)) {
    set_name_safe(0x000001CE, "DIRQ_DYNALOOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000001D0)) {
    set_name_safe(0x000001D0, "DIRQ_DYNREG");
    named = named + 1;
  }
  if (in_mapped_memory(0x000001DD)) {
    force_data_word(0x000001DD);
    marked_data = marked_data + 1;
    set_name_safe(0x000001DD, "COLLA_SCOLLF");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000246)) {
    set_name_safe(0x00000246, "DIRQ_EODVCTR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000247)) {
    set_name_safe(0x00000247, "DIRQ_DYNALPX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000250)) {
    set_name_safe(0x00000250, "DIRQ_DYNASHD");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000282)) {
    set_name_safe(0x00000282, "DIRQ_EOSVCTR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000285)) {
    set_name_safe(0x00000285, "DIRQ_NOSHAD");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000028D)) {
    set_name_safe(0x0000028D, "DIRQ_PLOTPOLY");
    named = named + 1;
  }
  if (in_mapped_memory(0x000002C0)) {
    set_name_safe(0x000002C0, "DIRQ_PLOTPOLYLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000002D5)) {
    set_name_safe(0x000002D5, "DIRQ_INBNDS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000002FD)) {
    set_name_safe(0x000002FD, "DIRQ_POLYLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000036F)) {
    set_name_safe(0x0000036F, "DIRQ_CLIPOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000370)) {
    set_name_safe(0x00000370, "DIRQ_CLIP0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000399)) {
    set_name_safe(0x00000399, "DIRQ_CLCKLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000003A1)) {
    set_name_safe(0x000003A1, "DIRQ_PCOUT");
    set_cmt(0x000003A1, "alts: DIRQ_PCWT", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x000003BD)) {
    set_name_safe(0x000003BD, "DIRQ_BUSTUP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000448)) {
    set_name_safe(0x00000448, "DIRQ_CLIPX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000044A)) {
    set_name_safe(0x0000044A, "DIRQ_CLIPDONE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000453)) {
    set_name_safe(0x00000453, "DIRQ_PLTPOLY");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000460)) {
    set_name_safe(0x00000460, "DIRQ_PLTPOLYLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000463)) {
    set_name_safe(0x00000463, "DIRQ_PLTPOLYLP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000046A)) {
    set_name_safe(0x0000046A, "DIRQ_PLTWT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000498)) {
    set_name_safe(0x00000498, "DIRQ_PLOT1PAL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000004C0)) {
    set_name_safe(0x000004C0, "DIRQ_PLOTPOLYLP_1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000004D5)) {
    set_name_safe(0x000004D5, "DIRQ_INBNDS_1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000004FC)) {
    set_name_safe(0x000004FC, "DIRQ_POLYLP_1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000506)) {
    set_name_safe(0x00000506, "DIRQ_PLT1PAL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000515)) {
    set_name_safe(0x00000515, "DIRQ_PLT1PAL1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000520)) {
    set_name_safe(0x00000520, "DIRQ_PLTPOLYLP_2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000523)) {
    set_name_safe(0x00000523, "DIRQ_PLTPOLYLP1_2");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000052A)) {
    set_name_safe(0x0000052A, "DIRQ_PLTWT_2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000557)) {
    set_name_safe(0x00000557, "DIRQ_PLTG1PAL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000560)) {
    set_name_safe(0x00000560, "DIRQ_PLTGPOLYLP_2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000563)) {
    set_name_safe(0x00000563, "DIRQ_PLTGPOLYLP1_2");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000056D)) {
    set_name_safe(0x0000056D, "DIRQ_PLTGWT_2");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000059E)) {
    set_name_safe(0x0000059E, "DIRQ_PLOTILLUM");
    named = named + 1;
  }
  if (in_mapped_memory(0x000005F5)) {
    force_data_word(0x000005F5);
    marked_data = marked_data + 1;
    set_name_safe(0x000005F5, "DIRQ_CLIPRAMI");
    named = named + 1;
  }
  if (in_mapped_memory(0x000005F6)) {
    set_name_safe(0x000005F6, "DIAG_WT_START");
    set_cmt(0x000005F6, "alts: DIAG_WT_S1", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x000005FA)) {
    set_name_safe(0x000005FA, "DIAG_WT_NSTART");
    set_cmt(0x000005FA, "alts: DIAG_WT_NS1", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00000600)) {
    set_name_safe(0x00000600, "DIAG_BEEP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000602)) {
    set_name_safe(0x00000602, "DIAG_MAKE_WINDOW");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000628)) {
    set_name_safe(0x00000628, "DIAG_MAKE_IWINDOW");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000678)) {
    set_name_safe(0x00000678, "DIAG_ENTER_DIAG_INIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000699)) {
    set_name_safe(0x00000699, "ENTER_DIAG");
    named = named + 1;
  }
  if (in_mapped_memory(0x000006A8)) {
    set_name_safe(0x000006A8, "TOEXIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000A4A)) {
    force_data_word(0x00000A4A);
    marked_data = marked_data + 1;
    set_name_safe(0x00000A4A, "DIAG_MMT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000A4F)) {
    set_name_safe(0x00000A4F, "DIAG_HANDLE_MENU");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000A52)) {
    set_name_safe(0x00000A52, "DIAG_HMER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000A6A)) {
    set_name_safe(0x00000A6A, "DIAG_HMSTAND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000A6F)) {
    set_name_safe(0x00000A6F, "DIAG_HMNOTSTAND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000AE9)) {
    set_name_safe(0x00000AE9, "SETUP_DIAG_SCREEN");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000AEF)) {
    set_name_safe(0x00000AEF, "DIAG_SNDMAC");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000CC0)) {
    set_name_safe(0x00000CC0, "DIAG_SET_CONTROLS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000D1E)) {
    set_name_safe(0x00000D1E, "DIAG_DADA5");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000DCE)) {
    set_name_safe(0x00000DCE, "DIAG_FFRSUB");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000DDA)) {
    force_data_word(0x00000DDA);
    marked_data = marked_data + 1;
    set_name_safe(0x00000DDA, "DIAG_FFRST");
    named = named + 1;
  }
  if (in_mapped_memory(0x00000E62)) {
    set_name_safe(0x00000E62, "DIAG_MESSAGE_BOX");
    set_cmt(0x00000E62, "alts: DIAG_CCD2A", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00000E73)) {
    set_name_safe(0x00000E73, "DIAG_ABLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001159)) {
    set_name_safe(0x00001159, "DIAG_LAMPSOFF");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000115D)) {
    set_name_safe(0x0000115D, "DIAG_LAMPS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000115F)) {
    set_name_safe(0x0000115F, "DIAG_LAMPV1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001161)) {
    set_name_safe(0x00001161, "DIAG_LAMPV2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001163)) {
    set_name_safe(0x00001163, "DIAG_LAMPV3");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001165)) {
    set_name_safe(0x00001165, "DIAG_LAMPLT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001167)) {
    set_name_safe(0x00001167, "DIAG_LAMPRT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001169)) {
    set_name_safe(0x00001169, "DIAG_LAMPLF");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000116B)) {
    set_name_safe(0x0000116B, "DIAG_LAMPRF");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000116C)) {
    set_name_safe(0x0000116C, "DIAG_LJIN");
    named = named + 1;
  }
  if (in_mapped_memory(0x000014D0)) {
    set_name_safe(0x000014D0, "AUDIT_DISPLAY");
    named = named + 1;
  }
  if (in_mapped_memory(0x000014DF)) {
    set_name_safe(0x000014DF, "AUDITS_KKLL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000014F8)) {
    set_name_safe(0x000014F8, "AUDITS_LYTL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001500)) {
    set_name_safe(0x00001500, "AUDITS_LLTT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000150E)) {
    set_name_safe(0x0000150E, "AUDITS_NOCALL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000150F)) {
    set_name_safe(0x0000150F, "AUDITS_J22AB");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000151B)) {
    set_name_safe(0x0000151B, "AUDITS_JJTTA");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000151D)) {
    set_name_safe(0x0000151D, "AUDITS_LLRR");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000151F)) {
    set_name_safe(0x0000151F, "AUDITS_WL2A");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000152A)) {
    set_name_safe(0x0000152A, "AUDITS_NODIPPLOT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001535)) {
    set_name_safe(0x00001535, "AUDITS_NOLOG");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001539)) {
    set_name_safe(0x00001539, "AUDITS_DBNCE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000153D)) {
    set_name_safe(0x0000153D, "AUDITS_FFB2");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000153F)) {
    set_name_safe(0x0000153F, "AUDITS_RB44");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001544)) {
    set_name_safe(0x00001544, "AUDITS_KKLL5");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001548)) {
    set_name_safe(0x00001548, "AUDITS_KKL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00001549)) {
    set_name_safe(0x00001549, "AUDITS_BOOKX");
    set_cmt(0x00001549, "alts: AUDITS_WL266", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x000021D0)) {
    set_name_safe(0x000021D0, "COLSCC");
    named = named + 1;
  }
  if (in_mapped_memory(0x000021DF)) {
    set_name_safe(0x000021DF, "COLLA_COLSCCX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000021E0)) {
    set_name_safe(0x000021E0, "COLLA_ATTR_COLLISION");
    named = named + 1;
  }
  if (in_mapped_memory(0x000021E8)) {
    set_name_safe(0x000021E8, "COLLA_NO_ATTR_COL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000021E9)) {
    set_name_safe(0x000021E9, "COLLA_PLYRDEBRIS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000021EC)) {
    set_name_safe(0x000021EC, "COLLA_PLYRSIGN");
    named = named + 1;
  }
  if (in_mapped_memory(0x000021EF)) {
    set_name_safe(0x000021EF, "COLLA_DRONDEBRIS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000021F1)) {
    set_name_safe(0x000021F1, "COLLA_DRONSIGN");
    named = named + 1;
  }
  if (in_mapped_memory(0x000021F2)) {
    set_name_safe(0x000021F2, "COLLA_DRONEPT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000021F5)) {
    set_name_safe(0x000021F5, "COLLA_DRONEPTL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002203)) {
    set_name_safe(0x00002203, "COLLA_DRONEPT1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002207)) {
    set_name_safe(0x00002207, "COLLA_COLPOINT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000220B)) {
    set_name_safe(0x0000220B, "COLLA_CARSCLP0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002215)) {
    set_name_safe(0x00002215, "COLLA_CARSCL0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002218)) {
    set_name_safe(0x00002218, "COLLA_CARSCL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000221F)) {
    set_name_safe(0x0000221F, "COLSGCK");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002233)) {
    set_name_safe(0x00002233, "COLLA_CSGLNEQ");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000225C)) {
    set_name_safe(0x0000225C, "COLLA_NOTCOCONUT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000225F)) {
    set_name_safe(0x0000225F, "COLLA_DOREPEL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000226F)) {
    set_name_safe(0x0000226F, "COLLA_HARDCOL00");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000227B)) {
    set_name_safe(0x0000227B, "COLLA_HARDCOL1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002280)) {
    set_name_safe(0x00002280, "COLLA_HARDCOL2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000022A9)) {
    set_name_safe(0x000022A9, "COLLA_HARDCOL3");
    named = named + 1;
  }
  if (in_mapped_memory(0x000022AC)) {
    set_name_safe(0x000022AC, "COLLA_ROADKILL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000022B2)) {
    set_name_safe(0x000022B2, "COLLA_FLYCOLL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000022D9)) {
    set_name_safe(0x000022D9, "COLLA_CLLL1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000022FA)) {
    set_name_safe(0x000022FA, "COLLA_RUNOVER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002321)) {
    set_name_safe(0x00002321, "COLLA_RUNOV0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002325)) {
    set_name_safe(0x00002325, "COLLA_RUNOV00");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002329)) {
    set_name_safe(0x00002329, "COLLA_COLSGCX0");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000232A)) {
    set_name_safe(0x0000232A, "COLLA_COLSGCX");
    set_cmt(0x0000232A, "alts: COLLA_SIGN_IGNORE", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x0000232D)) {
    set_name_safe(0x0000232D, "COLLA_SAGETAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000232E)) {
    force_data_word(0x0000232E);
    marked_data = marked_data + 1;
    set_name_safe(0x0000232E, "COLLA_SAGESND");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000238C)) {
    set_name_safe(0x0000238C, "COLLA_DEBSCAN");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000238E)) {
    set_name_safe(0x0000238E, "COLLA_DEBSCL0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002397)) {
    set_name_safe(0x00002397, "COLLA_DEBSCL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002398)) {
    set_name_safe(0x00002398, "COLLA_DEBSCL1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000023D1)) {
    set_name_safe(0x000023D1, "COLLA_FREESIGN");
    named = named + 1;
  }
  if (in_mapped_memory(0x000023D4)) {
    set_name_safe(0x000023D4, "COLLA_SFREELP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000023EF)) {
    set_name_safe(0x000023EF, "COLLA_FLYCAR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002405)) {
    set_name_safe(0x00002405, "COLLA_FLYCAR0");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000240E)) {
    set_name_safe(0x0000240E, "FLYCAR1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000242D)) {
    set_name_safe(0x0000242D, "COLLA_L78G");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002440)) {
    set_name_safe(0x00002440, "COLLA_FLY0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002445)) {
    set_name_safe(0x00002445, "COLLA_FLY1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000244B)) {
    set_name_safe(0x0000244B, "COLLA_FLY3");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002454)) {
    set_name_safe(0x00002454, "COLLA_FLYCARX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002458)) {
    set_name_safe(0x00002458, "COLLA_FLYCARXX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000245D)) {
    set_name_safe(0x0000245D, "COLLA_FC00");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002460)) {
    set_name_safe(0x00002460, "COLLA_FC01");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002462)) {
    set_name_safe(0x00002462, "COLLA_FC02");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002463)) {
    set_name_safe(0x00002463, "COLLA_FC03");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000246A)) {
    set_name_safe(0x0000246A, "COLLA_DETHTAB1");
    set_cmt(0x0000246A, "alts: COLLA_DETHTAB2", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00002473)) {
    force_data_word(0x00002473);
    marked_data = marked_data + 1;
    set_name_safe(0x00002473, "COLLA_MDETHSCREAM2");
    set_cmt(0x00002473, "alts: COLLA_MFDETHSCREAM1", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x000025E8)) {
    set_name_safe(0x000025E8, "COLSCAN");
    named = named + 1;
  }
  if (in_mapped_memory(0x000025EC)) {
    set_name_safe(0x000025EC, "COLLA_COLSCLP0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002604)) {
    set_name_safe(0x00002604, "COLLA_COLSCL0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002608)) {
    set_name_safe(0x00002608, "COLLA_COLSCL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000260E)) {
    set_name_safe(0x0000260E, "COLLA_CLDSCAN");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002614)) {
    set_name_safe(0x00002614, "COLLA_CLDSCLP0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002621)) {
    set_name_safe(0x00002621, "COLLA_CLDSCL0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002628)) {
    set_name_safe(0x00002628, "COLLA_CLDSCL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000262D)) {
    set_name_safe(0x0000262D, "COLLA_CLDSCL1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002633)) {
    set_name_safe(0x00002633, "COLLA_REPELL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002645)) {
    set_name_safe(0x00002645, "COLLA_COLDISP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002659)) {
    set_name_safe(0x00002659, "COLLA_NOTHELI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000265F)) {
    set_name_safe(0x0000265F, "COLLA_NTRN");
    named = named + 1;
  }
  if (in_mapped_memory(0x000026A2)) {
    set_name_safe(0x000026A2, "COLLA_COLDISP0");
    named = named + 1;
  }
  if (in_mapped_memory(0x000026AA)) {
    set_name_safe(0x000026AA, "COLLA_COLIN1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000026FC)) {
    set_name_safe(0x000026FC, "COLLA_ZZZ1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002727)) {
    set_name_safe(0x00002727, "COLLA_SPINROT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002833)) {
    set_name_safe(0x00002833, "COLLA_CKBNCX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002835)) {
    set_name_safe(0x00002835, "COLLA_COLSND");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000284B)) {
    set_name_safe(0x0000284B, "COLLA_COLSND1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000284F)) {
    set_name_safe(0x0000284F, "COLLA_DRCOLSND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002853)) {
    set_name_safe(0x00002853, "COLLA_COLSNDX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002862)) {
    set_name_safe(0x00002862, "COLLA_SCUPDTAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002863)) {
    set_name_safe(0x00002863, "COLCHK");
    set_cmt(0x00002863, "alts: COLLA_SCTAB", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x0000289F)) {
    set_name_safe(0x0000289F, "COLLA_COLCHKX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000028A8)) {
    set_name_safe(0x000028A8, "COLLA_GETBOX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002F8B)) {
    force_data_word(0x00002F8B);
    marked_data = marked_data + 1;
    set_name_safe(0x00002F8B, "TEXT_dnums_I");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002FB5)) {
    force_data_word(0x00002FB5);
    marked_data = marked_data + 1;
    set_name_safe(0x00002FB5, "TEXT_osg10fnt_I");
    named = named + 1;
  }
  if (in_mapped_memory(0x00002FCF)) {
    force_data_word(0x00002FCF);
    marked_data = marked_data + 1;
    set_name_safe(0x00002FCF, "TEXT_ommdfont_I");
    named = named + 1;
  }
  if (in_mapped_memory(0x00003031)) {
    set_name_safe(0x00003031, "PLYR_GETNXTRDIR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00003037)) {
    set_name_safe(0x00003037, "PLYR_ROADIR");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000303B)) {
    set_name_safe(0x0000303B, "PLYR_GETRD1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00003049)) {
    set_name_safe(0x00003049, "PLYR_ROADIRX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000305B)) {
    force_data_word(0x0000305B);
    marked_data = marked_data + 1;
    set_name_safe(0x0000305B, "TEXT_font18_I");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000307F)) {
    force_data_word(0x0000307F);
    marked_data = marked_data + 1;
    set_name_safe(0x0000307F, "TEXT_lgnum43_I");
    named = named + 1;
  }
  if (in_mapped_memory(0x00003159)) {
    set_name_safe(0x00003159, "PLYR_RANDSND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00003160)) {
    set_name_safe(0x00003160, "PLYR_RANDVSND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00003168)) {
    set_name_safe(0x00003168, "DRONESND");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000316E)) {
    set_name_safe(0x0000316E, "DRONESND1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004541)) {
    set_name_safe(0x00004541, "INITIALIZATION_MOTION_CHECK");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004569)) {
    set_name_safe(0x00004569, "MOTION_JAJA66");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000456B)) {
    set_name_safe(0x0000456B, "MOTION_JAJA887");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000456E)) {
    set_name_safe(0x0000456E, "MOTION_GLLLT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004626)) {
    set_name_safe(0x00004626, "MOTION_SEND_MOTOR_OFF_NO_RESET");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004632)) {
    set_name_safe(0x00004632, "MOTION_MOTOFF_LOOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000464E)) {
    set_name_safe(0x0000464E, "MOTION_MOTOFF_DONE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004655)) {
    set_name_safe(0x00004655, "MOTION_INIT_MOTION_ERROROUT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004660)) {
    set_name_safe(0x00004660, "MOTION_GDLPAA");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004691)) {
    set_name_safe(0x00004691, "MOTION_JJG");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000469A)) {
    set_name_safe(0x0000469A, "MOTION_WAIT500");
    named = named + 1;
  }
  if (in_mapped_memory(0x000046A2)) {
    set_name_safe(0x000046A2, "MOTION_WT500X");
    named = named + 1;
  }
  if (in_mapped_memory(0x000046A4)) {
    set_name_safe(0x000046A4, "MOTION_CHECK_MOTION_DIP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000046AE)) {
    set_name_safe(0x000046AE, "MOTION_CHECK_MOTION_PRESENT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004845)) {
    set_name_safe(0x00004845, "LEVEL_THE_MOTION");
    named = named + 1;
  }
  if (in_mapped_memory(0x000048D5)) {
    set_name_safe(0x000048D5, "MOTION_IBO654");
    named = named + 1;
  }
  if (in_mapped_memory(0x000048DC)) {
    set_name_safe(0x000048DC, "MOTION_DNF");
    named = named + 1;
  }
  if (in_mapped_memory(0x000048E3)) {
    set_name_safe(0x000048E3, "MOTION_SEND_CMD");
    named = named + 1;
  }
  if (in_mapped_memory(0x000048FF)) {
    set_name_safe(0x000048FF, "MOTION_LLKK");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004912)) {
    set_name_safe(0x00004912, "MOTION_KKKII");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000491C)) {
    set_name_safe(0x0000491C, "RESET_GALIL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000049A2)) {
    set_name_safe(0x000049A2, "MOTION_WAITMSEC");
    named = named + 1;
  }
  if (in_mapped_memory(0x000049A3)) {
    set_name_safe(0x000049A3, "MOTION_WTMLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000049A6)) {
    force_data_word(0x000049A6);
    marked_data = marked_data + 1;
    set_name_safe(0x000049A6, "MOTION_CME_MASK");
    named = named + 1;
  }
  if (in_mapped_memory(0x000049A7)) {
    set_name_safe(0x000049A7, "MOTION_CHECK_MOTION_ERROR");
    named = named + 1;
  }
  if (in_mapped_memory(0x000049B4)) {
    set_name_safe(0x000049B4, "MOTION_CME_NO_MOTION_ERRORS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000049B8)) {
    set_name_safe(0x000049B8, "MOTION_CLEAR_LATCH_ERROR");
    named = named + 1;
  }
  if (in_mapped_memory(0x000049C9)) {
    set_name_safe(0x000049C9, "MOTION_G_READY");
    named = named + 1;
  }
  if (in_mapped_memory(0x000049DF)) {
    set_name_safe(0x000049DF, "MOTION_G_CHAR");
    named = named + 1;
  }
  if (in_mapped_memory(0x000049FB)) {
    set_name_safe(0x000049FB, "MOTION_G_STRING");
    set_cmt(0x000049FB, "alts: MOTION_GS0", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A06)) {
    set_name_safe(0x00004A06, "MOTION_NOTPERR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A11)) {
    set_name_safe(0x00004A11, "MOTION_NOTQSTATUS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A15)) {
    set_name_safe(0x00004A15, "MOTION_NOTQQERRORX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A19)) {
    set_name_safe(0x00004A19, "MOTION_NOTQQERRORY");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A1D)) {
    set_name_safe(0x00004A1D, "MOTION_SHOWENCODER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A2C)) {
    set_name_safe(0x00004A2C, "MOTION_NOTQQERRORZ");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A32)) {
    set_name_safe(0x00004A32, "MOTION_GS1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A38)) {
    set_name_safe(0x00004A38, "MOTION_GS2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A3E)) {
    set_name_safe(0x00004A3E, "MOTION_GSX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A3F)) {
    set_name_safe(0x00004A3F, "MOTION_G_HEX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A50)) {
    set_name_safe(0x00004A50, "MOTION_GHX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A52)) {
    set_name_safe(0x00004A52, "MOTION_ASCIIHEX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A5B)) {
    set_name_safe(0x00004A5B, "MOTION_AH1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A5D)) {
    set_name_safe(0x00004A5D, "MOTION_ASCERR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A5F)) {
    set_name_safe(0x00004A5F, "MOTION_WAIT_ACK");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A66)) {
    set_name_safe(0x00004A66, "MOTION_WAIT_ACK_LOOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A6F)) {
    set_name_safe(0x00004A6F, "MOTION_EXITL2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A71)) {
    set_name_safe(0x00004A71, "MOTION_NO_RESPONSE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A76)) {
    set_name_safe(0x00004A76, "MOTION_WAIT_ACK_REAL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A77)) {
    set_name_safe(0x00004A77, "MOTION_WAR0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A7C)) {
    set_name_safe(0x00004A7C, "MOTION_WAR1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A7D)) {
    set_name_safe(0x00004A7D, "MOTION_WAR1L");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004A82)) {
    set_name_safe(0x00004A82, "MOTION_EXITL2A");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004AE6)) {
    force_data_word(0x00004AE6);
    marked_data = marked_data + 1;
    set_name_safe(0x00004AE6, "_c_int00");
    set_cmt(0x00004AE6, "alts: COMP_DECODE_STACK, PALL_PALRAM, PALL_PTTRAM, PALL_RAWLOCS, _PALLIST", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00004B18)) {
    set_name_safe(0x00004B18, "CUSA_LD_RAM");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004B39)) {
    set_name_safe(0x00004B39, "DIAG_RETURN");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004B3C)) {
    set_name_safe(0x00004B3C, "CUSA_DR1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004B50)) {
    set_name_safe(0x00004B50, "CUSA_LD_RAM2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004BAE)) {
    set_name_safe(0x00004BAE, "CUSA_NOPEIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004BCA)) {
    set_name_safe(0x00004BCA, "CUSA_NODO1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004BD1)) {
    set_name_safe(0x00004BD1, "CUSA_MAINLOOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004BDF)) {
    set_name_safe(0x00004BDF, "CUSA_MWAIT0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004BE5)) {
    set_name_safe(0x00004BE5, "CUSA_MWAIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004C01)) {
    set_name_safe(0x00004C01, "CUSA_DOIT8");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004C02)) {
    set_name_safe(0x00004C02, "CUSA_NHUD");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004C0E)) {
    set_name_safe(0x00004C0E, "CUSA_NODO555");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004C2C)) {
    set_name_safe(0x00004C2C, "COLD_ENTER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004C3B)) {
    set_name_safe(0x00004C3B, "CUSA_C_WAIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004C44)) {
    set_name_safe(0x00004C44, "ENABLEGIE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004D00)) {
    set_name_safe(0x00004D00, "CUSA_READIO");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004D38)) {
    set_name_safe(0x00004D38, "CUSA_NIGY");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004D39)) {
    set_name_safe(0x00004D39, "CUSA_VOL_MINUS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004D4E)) {
    set_name_safe(0x00004D4E, "CUSA_VOL_PLUS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004D62)) {
    set_name_safe(0x00004D62, "CUSA_VOLJN");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004D72)) {
    set_name_safe(0x00004D72, "CUSA_DIAG_TOGGLE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004D7E)) {
    set_name_safe(0x00004D7E, "CUSA_DTXX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004DBB)) {
    set_name_safe(0x00004DBB, "CUSA_RDFOOT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004DDB)) {
    set_name_safe(0x00004DDB, "CUSA_RDBRAKE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004DEA)) {
    set_name_safe(0x00004DEA, "CUSA_EXITR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004DF7)) {
    set_name_safe(0x00004DF7, "CUSA_SWDISP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004DFE)) {
    set_name_safe(0x00004DFE, "CUSA_SWTLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E08)) {
    set_name_safe(0x00004E08, "CUSA_NOGPROC");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E0A)) {
    set_name_safe(0x00004E0A, "CUSA_SWSTX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E0B)) {
    force_data_word(0x00004E0B);
    marked_data = marked_data + 1;
    set_name_safe(0x00004E0B, "CUSA_SWTABI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E0C)) {
    force_data_word(0x00004E0C);
    marked_data = marked_data + 1;
    set_name_safe(0x00004E0C, "CUSA_SWTAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E2C)) {
    set_name_safe(0x00004E2C, "CHECKDIAG");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E2F)) {
    force_data_word(0x00004E2F);
    marked_data = marked_data + 1;
    set_name_safe(0x00004E2F, "CUSA_PB1");
    set_cmt(0x00004E2F, "alts: CUSA_PB2, CUSA_PBSS_PTR", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E30)) {
    force_data_word(0x00004E30);
    marked_data = marked_data + 1;
    set_name_safe(0x00004E30, "CUSA_PBSS_BSSEND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E31)) {
    set_name_safe(0x00004E31, "CUSA_CLR_PBSS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E3C)) {
    set_name_safe(0x00004E3C, "CUSA_PRAMCLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E44)) {
    force_data_word(0x00004E44);
    marked_data = marked_data + 1;
    set_name_safe(0x00004E44, "CUSA_RAM_PTR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E45)) {
    force_data_word(0x00004E45);
    marked_data = marked_data + 1;
    set_name_safe(0x00004E45, "CUSA_RAM_BSSEND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E46)) {
    set_name_safe(0x00004E46, "CLR_RAM");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E51)) {
    set_name_safe(0x00004E51, "CUSA_RAMCLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E72)) {
    set_name_safe(0x00004E72, "CUSA_BUT3");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E83)) {
    set_name_safe(0x00004E83, "CUSA_NOSTART");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E87)) {
    set_name_safe(0x00004E87, "CUSA_PAPAFFD");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E88)) {
    set_name_safe(0x00004E88, "CUSA_BUTLITE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E9D)) {
    set_name_safe(0x00004E9D, "CUSA_KKDAK");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004E9F)) {
    set_name_safe(0x00004E9F, "CUSA_FDDDA");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004EAC)) {
    set_name_safe(0x00004EAC, "CUSA_DIAG_BUTTONS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004EBD)) {
    set_name_safe(0x00004EBD, "CUSA_DGBT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004ED8)) {
    set_name_safe(0x00004ED8, "WAIT_FOR_VBLANK");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004ED9)) {
    set_name_safe(0x00004ED9, "CUSA_WAIT1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004EDD)) {
    set_name_safe(0x00004EDD, "CUSA_CRT_REG_SETUP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004EE9)) {
    set_name_safe(0x00004EE9, "CUSA_CRTRGLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004EF4)) {
    set_name_safe(0x00004EF4, "FIFO_RESET");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004F0A)) {
    set_name_safe(0x00004F0A, "CUSA_TIMER_RESET");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004F18)) {
    force_data_word(0x00004F18);
    marked_data = marked_data + 1;
    set_name_safe(0x00004F18, "CUSA_TIMERAMI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004F19)) {
    set_name_safe(0x00004F19, "CUSA_TIMERESET");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004F2C)) {
    set_name_safe(0x00004F2C, "CUSA_TIMELP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004F2F)) {
    set_name_safe(0x00004F2F, "CUSA_TIMEL1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004F38)) {
    set_name_safe(0x00004F38, "CUSA_TIMEREC");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004F43)) {
    set_name_safe(0x00004F43, "CUSA_MESSAGE1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004F80)) {
    set_name_safe(0x00004F80, "CUSA_DODOIBO");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004F83)) {
    set_name_safe(0x00004F83, "CUSA_HJSADF");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004F9F)) {
    set_name_safe(0x00004F9F, "CUSA_MSG1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004FB1)) {
    set_name_safe(0x00004FB1, "CUSA_MSG2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004FBB)) {
    force_data_word(0x00004FBB);
    marked_data = marked_data + 1;
    set_name_safe(0x00004FBB, "CUSA_LINKDISABLED");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004FBC)) {
    force_data_word(0x00004FBC);
    marked_data = marked_data + 1;
    set_name_safe(0x00004FBC, "CUSA_IAMMASTER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004FBD)) {
    force_data_word(0x00004FBD);
    marked_data = marked_data + 1;
    set_name_safe(0x00004FBD, "CUSA_IAMSLAVE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004FBE)) {
    force_data_word(0x00004FBE);
    marked_data = marked_data + 1;
    set_name_safe(0x00004FBE, "CUSA_TPALI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004FBF)) {
    force_data_word(0x00004FBF);
    marked_data = marked_data + 1;
    set_name_safe(0x00004FBF, "CUSA_TPALNI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004FC0)) {
    set_name_safe(0x00004FC0, "CUSA_MSG3");
    named = named + 1;
  }
  if (in_mapped_memory(0x00004FFC)) {
    force_data_word(0x00004FFC);
    marked_data = marked_data + 1;
    set_name_safe(0x00004FFC, "CUSA_BUTTONI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00005001)) {
    set_name_safe(0x00005001, "CUSA_DASHLIGHT");
    set_cmt(0x00005001, "alts: CUSA_BUTTII", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00005014)) {
    set_name_safe(0x00005014, "CUSA_CMOS_ERROR");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000502B)) {
    set_name_safe(0x0000502B, "CUSA_VERSION_UPDATE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00005045)) {
    set_name_safe(0x00005045, "CUSA_FAKEDIAG");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000504E)) {
    set_name_safe(0x0000504E, "FEED_WATCHDOG");
    named = named + 1;
  }
  if (in_mapped_memory(0x00005061)) {
    set_name_safe(0x00005061, "CUSA_FEED_WATCHDOG_HARD");
    named = named + 1;
  }
  if (in_mapped_memory(0x00005568)) {
    set_name_safe(0x00005568, "RACER_WRECKST");
    named = named + 1;
  }
  if (in_mapped_memory(0x00005574)) {
    set_name_safe(0x00005574, "RACER_WRKST1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000060C0)) {
    force_data_word(0x000060C0);
    marked_data = marked_data + 1;
    set_name_safe(0x000060C0, "MAP_COLONI");
    named = named + 1;
  }
  if (in_mapped_memory(0x000060C1)) {
    force_data_word(0x000060C1);
    marked_data = marked_data + 1;
    set_name_safe(0x000060C1, "MAP_COLON");
    named = named + 1;
  }
  if (in_mapped_memory(0x000060C3)) {
    force_data_word(0x000060C3);
    marked_data = marked_data + 1;
    set_name_safe(0x000060C3, "MAP_tmp_bufferI");
    named = named + 1;
  }
  if (in_mapped_memory(0x000060C4)) {
    set_name_safe(0x000060C4, "TIME2STR");
    named = named + 1;
  }
  if (in_mapped_memory(0x000060E5)) {
    force_data_word(0x000060E5);
    marked_data = marked_data + 1;
    set_name_safe(0x000060E5, "MAP_MINFACT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000060E6)) {
    force_data_word(0x000060E6);
    marked_data = marked_data + 1;
    set_name_safe(0x000060E6, "MAP_SECFACT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000060E7)) {
    force_data_word(0x000060E7);
    marked_data = marked_data + 1;
    set_name_safe(0x000060E7, "MAP_HUNFACT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000060E8)) {
    set_name_safe(0x000060E8, "CVTTIME");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000610A)) {
    set_name_safe(0x0000610A, "RADAR_PLOT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006128)) {
    set_name_safe(0x00006128, "MAP_RADAR_LP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006180)) {
    set_name_safe(0x00006180, "MAP_GL14");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006197)) {
    set_name_safe(0x00006197, "MAP_GL15");
    named = named + 1;
  }
  if (in_mapped_memory(0x000061B0)) {
    set_name_safe(0x000061B0, "MAP_DADA4");
    named = named + 1;
  }
  if (in_mapped_memory(0x000061B4)) {
    set_name_safe(0x000061B4, "MAP_NOTTHEOPLYR");
    named = named + 1;
  }
  if (in_mapped_memory(0x000061CE)) {
    set_name_safe(0x000061CE, "MAP_RADAR_X");
    named = named + 1;
  }
  if (in_mapped_memory(0x000061EB)) {
    set_name_safe(0x000061EB, "MAP_NODOAP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000062FA)) {
    set_name_safe(0x000062FA, "MEMTEST_CHIPMAC");
    named = named + 1;
  }
  if (in_mapped_memory(0x000062FC)) {
    set_name_safe(0x000062FC, "MEMTEST_CHIPMC");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006300)) {
    set_name_safe(0x00006300, "TEST_STATIC_CHIPS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006322)) {
    set_name_safe(0x00006322, "MEMTEST_GET_AR4_DIGITS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006335)) {
    set_name_safe(0x00006335, "MEMTEST_R1R2ZER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006336)) {
    set_name_safe(0x00006336, "MEMTEST_R3ZERO");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006337)) {
    set_name_safe(0x00006337, "MEMTEST_NZERO");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006338)) {
    set_name_safe(0x00006338, "MEMTEST_BLINK_AND_CONTINUE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006349)) {
    set_name_safe(0x00006349, "MEMTEST_IS_STATIC_ERROR");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000634A)) {
    set_name_safe(0x0000634A, "MEMTEST_ELOOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000634C)) {
    set_name_safe(0x0000634C, "MEMTEST_LLED_ON");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006353)) {
    set_name_safe(0x00006353, "MEMTEST_LLED_OFF");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000635A)) {
    set_name_safe(0x0000635A, "MEMTEST_BLINK_DIGITS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006370)) {
    set_name_safe(0x00006370, "MEMTEST_BDL1X");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006386)) {
    set_name_safe(0x00006386, "MEMTEST_BDL2X");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000638F)) {
    set_name_safe(0x0000638F, "MEMTEST_BDL3");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000639C)) {
    set_name_safe(0x0000639C, "MEMTEST_BDL3X");
    named = named + 1;
  }
  if (in_mapped_memory(0x000063AC)) {
    set_name_safe(0x000063AC, "TEST_CHIPS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000063C4)) {
    set_name_safe(0x000063C4, "MEMTEST_TESTLP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000063D2)) {
    set_name_safe(0x000063D2, "MEMTEST_CHECK_AS_RAM");
    named = named + 1;
  }
  if (in_mapped_memory(0x000063E3)) {
    set_name_safe(0x000063E3, "MEMTEST_DN_CHK");
    named = named + 1;
  }
  if (in_mapped_memory(0x000063FB)) {
    set_name_safe(0x000063FB, "MEMTEST_TEST_CHIPSX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000642E)) {
    set_name_safe(0x0000642E, "MEMTEST_CMOS_CHIP_TEST");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000643E)) {
    set_name_safe(0x0000643E, "MEMTEST_KKLL1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006456)) {
    set_name_safe(0x00006456, "MEMTEST_CM_ISERROR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006458)) {
    set_name_safe(0x00006458, "MEMTEST_KKJJ");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006478)) {
    set_name_safe(0x00006478, "MEMTEST_TEST_BASICS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006527)) {
    set_name_safe(0x00006527, "MEMTEST_RAMTEST");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000656A)) {
    set_name_safe(0x0000656A, "MEMTEST_ENDING");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006573)) {
    set_name_safe(0x00006573, "MEMTEST_COPY_RAMTEST");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000657B)) {
    set_name_safe(0x0000657B, "MEMTEST_BBCP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000657D)) {
    set_name_safe(0x0000657D, "MEMTEST_GENERATE_CHECKSUM");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000677F)) {
    set_name_safe(0x0000677F, "INIT_DRONES");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006946)) {
    set_name_safe(0x00006946, "PLYRROADKILL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006953)) {
    set_name_safe(0x00006953, "ROADKILL_PLYRKX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006957)) {
    set_name_safe(0x00006957, "ROADKILL_CHECK_COLLISION");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006967)) {
    set_name_safe(0x00006967, "ROADKILL_CCOLLX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006968)) {
    set_name_safe(0x00006968, "ROADKILL_CHECK_OFFSET");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006982)) {
    set_name_safe(0x00006982, "ROADKILL_COFFX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006984)) {
    set_name_safe(0x00006984, "ROADKILL_FLYERP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000698F)) {
    set_name_safe(0x0000698F, "ROADKILL_NORMAL_ROADKILL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006997)) {
    set_name_safe(0x00006997, "ROADKILL_RKFP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006998)) {
    set_name_safe(0x00006998, "ROADKILL_RKFPX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000699C)) {
    set_name_safe(0x0000699C, "ROADKILL_HIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000069B7)) {
    set_name_safe(0x000069B7, "ROADKILL_DO_PLYR");
    named = named + 1;
  }
  if (in_mapped_memory(0x000069BD)) {
    set_name_safe(0x000069BD, "ROADKILL_PKQ");
    named = named + 1;
  }
  if (in_mapped_memory(0x000069C2)) {
    set_name_safe(0x000069C2, "ROADKILL_SETKILL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000069D0)) {
    set_name_safe(0x000069D0, "ROADKILL_NNWAY");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006A91)) {
    set_name_safe(0x00006A91, "DEER_EXPLODE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006A9C)) {
    set_name_safe(0x00006A9C, "ROADKILL_MAKE_PARTSLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006AB3)) {
    set_name_safe(0x00006AB3, "ROADKILL_DEER_EXPX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006AC1)) {
    set_name_safe(0x00006AC1, "ROADKILL_NO_NEG");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006ACC)) {
    set_name_safe(0x00006ACC, "ROADKILL_NO_SPINOUT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006AD1)) {
    set_name_safe(0x00006AD1, "ROADKILL_MAKE_NOCOLL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006AD6)) {
    set_name_safe(0x00006AD6, "ROADKILL_DEERBLOOD_ANI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006AFE)) {
    set_name_safe(0x00006AFE, "FLYING_PARTS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006B04)) {
    set_name_safe(0x00006B04, "ROADKILL_FLY_PARTS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00006B3E)) {
    set_name_safe(0x00006B3E, "ROADKILL_FLY_PARTSX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007014)) {
    force_data_word(0x00007014);
    marked_data = marked_data + 1;
    set_name_safe(0x00007014, "OBJ_OFREEI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007015)) {
    force_data_word(0x00007015);
    marked_data = marked_data + 1;
    set_name_safe(0x00007015, "OBJ_OBJSTRI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007016)) {
    set_name_safe(0x00007016, "OBJ_OBJ_INIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000702E)) {
    set_name_safe(0x0000702E, "OBJ_OINITL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007035)) {
    set_name_safe(0x00007035, "OBJ_OBJ_GETE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007042)) {
    set_name_safe(0x00007042, "OBJ_NOT_ILLUM");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000704C)) {
    set_name_safe(0x0000704C, "OBJ_NOT_1PAL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007050)) {
    set_name_safe(0x00007050, "OBJ_OBJ_GET");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007078)) {
    set_name_safe(0x00007078, "OBJ_NOOBJ");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007083)) {
    set_name_safe(0x00007083, "OBJ_GETDIST");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070BE)) {
    set_name_safe(0x000070BE, "OBJ_OBJ_INSERT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070C8)) {
    set_name_safe(0x000070C8, "OBJ_INSIDLE");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070D0)) {
    set_name_safe(0x000070D0, "OBJ_INSACT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070D5)) {
    set_name_safe(0x000070D5, "OBJ_INSOBJL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070DB)) {
    set_name_safe(0x000070DB, "OBJ_INS_AT_END");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070DD)) {
    set_name_safe(0x000070DD, "OBJ_INSOBJX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070E6)) {
    set_name_safe(0x000070E6, "OBJ_OBJ_FIND_FIRST");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070E8)) {
    set_name_safe(0x000070E8, "OBJ_L89");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070EA)) {
    set_name_safe(0x000070EA, "OBJ_FF_LP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070EF)) {
    set_name_safe(0x000070EF, "OBJ_FF_ERR");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070F2)) {
    set_name_safe(0x000070F2, "OBJ_FF_OK");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070F5)) {
    set_name_safe(0x000070F5, "OBJ_OBJ_FREE_GROUND");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070FA)) {
    set_name_safe(0x000070FA, "OBJ_OBJ_FREE_SIGN");
    named = named + 1;
  }
  if (in_mapped_memory(0x000070FE)) {
    set_name_safe(0x000070FE, "OBJ_DELSLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000710D)) {
    set_name_safe(0x0000710D, "OBJ_NOT_ON_SUPPXLIST");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007110)) {
    set_name_safe(0x00007110, "OBJ_OBJ_FREE_DRIVE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007114)) {
    set_name_safe(0x00007114, "OBJ_DELRLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007123)) {
    set_name_safe(0x00007123, "OBJ_NOT_ON_SUPPROADLIST");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007126)) {
    set_name_safe(0x00007126, "OBJ_OBJ_FREE_PROC");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000712B)) {
    set_name_safe(0x0000712B, "OBJ_OBJ_DELETE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007134)) {
    set_name_safe(0x00007134, "OBJ_DYNDEL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000713B)) {
    set_name_safe(0x0000713B, "OBJ_NOTDYNAMIC");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000718A)) {
    set_name_safe(0x0000718A, "OBJ_OBJ_PULL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000071B2)) {
    set_name_safe(0x000071B2, "OBJ_ZSORTWT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000071B4)) {
    set_name_safe(0x000071B4, "OBJ_ZSORTWL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000071BC)) {
    set_name_safe(0x000071BC, "OBJ_ZSWTLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000071CE)) {
    set_name_safe(0x000071CE, "OBJ_ZWPRIOK");
    named = named + 1;
  }
  if (in_mapped_memory(0x000071D4)) {
    set_name_safe(0x000071D4, "OBJ_ZSWTX");
    set_cmt(0x000071D4, "alts: OBJ_ZSWTXX", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x000071DE)) {
    set_name_safe(0x000071DE, "OBJ_PLYRDLINK");
    named = named + 1;
  }
  if (in_mapped_memory(0x000071DF)) {
    set_name_safe(0x000071DF, "OBJ_PSORTNXT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000071E8)) {
    set_name_safe(0x000071E8, "OBJ_PDLINKX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000071EA)) {
    set_name_safe(0x000071EA, "OBJ_PLYRSORT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000071F3)) {
    set_name_safe(0x000071F3, "OBJ_PSRT1L");
    named = named + 1;
  }
  if (in_mapped_memory(0x000071FF)) {
    set_name_safe(0x000071FF, "OBJ_PSRT2A");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007203)) {
    set_name_safe(0x00007203, "OBJ_PSRT2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007208)) {
    set_name_safe(0x00007208, "OBJ_PSRT1NXT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000720E)) {
    set_name_safe(0x0000720E, "OBJ_PSORTX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000720F)) {
    set_name_safe(0x0000720F, "OBJ_DRONESORT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007222)) {
    set_name_safe(0x00007222, "OBJ_NXTDRONE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000724E)) {
    set_name_safe(0x0000724E, "OBJ_DSORTL1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007250)) {
    set_name_safe(0x00007250, "OBJ_DSORTL1X");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007256)) {
    set_name_safe(0x00007256, "OBJ_DSORTL2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007259)) {
    set_name_safe(0x00007259, "OBJ_DZSORTUP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007261)) {
    set_name_safe(0x00007261, "OBJ_DSLP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007265)) {
    set_name_safe(0x00007265, "OBJ_DSL000");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007267)) {
    set_name_safe(0x00007267, "OBJ_DSL00");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007268)) {
    set_name_safe(0x00007268, "OBJ_DSL11");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000726E)) {
    set_name_safe(0x0000726E, "OBJ_DZSORTUP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007270)) {
    set_name_safe(0x00007270, "OBJ_DSLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007272)) {
    set_name_safe(0x00007272, "OBJ_DSL1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007277)) {
    set_name_safe(0x00007277, "OBJ_DSDONE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000727C)) {
    set_name_safe(0x0000727C, "OBJ_DSORTXX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000727E)) {
    force_data_word(0x0000727E);
    marked_data = marked_data + 1;
    set_name_safe(0x0000727E, "OBJ_ACTIVEHI");
    named = named + 1;
  }
  if (in_mapped_memory(0x000073CB)) {
    set_name_safe(0x000073CB, "COIN_GETCOIN");
    named = named + 1;
  }
  if (in_mapped_memory(0x000073D1)) {
    set_name_safe(0x000073D1, "COIN_USE_STANDARD");
    named = named + 1;
  }
  if (in_mapped_memory(0x000073DA)) {
    set_name_safe(0x000073DA, "COIN_GETCOINX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000073DE)) {
    set_name_safe(0x000073DE, "GETCOINTXT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000073E2)) {
    set_name_safe(0x000073E2, "COIN_GETCOIN_DEFAULT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000073EF)) {
    set_name_safe(0x000073EF, "COIN_INVBLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007434)) {
    set_name_safe(0x00007434, "INIT_CUSTOM_COIN");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000744C)) {
    set_name_safe(0x0000744C, "COIN_INICC");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000744D)) {
    set_name_safe(0x0000744D, "COIN_INICC_LP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007458)) {
    force_data_word(0x00007458);
    marked_data = marked_data + 1;
    set_name_safe(0x00007458, "COIN_CUSTOM_COINTABI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000745B)) {
    force_data_word(0x0000745B);
    marked_data = marked_data + 1;
    set_name_safe(0x0000745B, "COIN_CTS_STRI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000745C)) {
    force_data_word(0x0000745C);
    marked_data = marked_data + 1;
    set_name_safe(0x0000745C, "COIN_CTS_STR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007466)) {
    force_data_word(0x00007466);
    marked_data = marked_data + 1;
    set_name_safe(0x00007466, "COIN_CREDITBUFFI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007467)) {
    force_data_word(0x00007467);
    marked_data = marked_data + 1;
    set_name_safe(0x00007467, "COIN_TOSTARTBUFFI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000746A)) {
    force_data_word(0x0000746A);
    marked_data = marked_data + 1;
    set_name_safe(0x0000746A, "COIN_SPCI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000746B)) {
    force_data_word(0x0000746B);
    marked_data = marked_data + 1;
    set_name_safe(0x0000746B, "COIN_SPC");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000746E)) {
    force_data_word(0x0000746E);
    marked_data = marked_data + 1;
    set_name_safe(0x0000746E, "COIN_CWI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000746F)) {
    force_data_word(0x0000746F);
    marked_data = marked_data + 1;
    set_name_safe(0x0000746F, "COIN_CW");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007471)) {
    force_data_word(0x00007471);
    marked_data = marked_data + 1;
    set_name_safe(0x00007471, "COIN_CWSI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007472)) {
    force_data_word(0x00007472);
    marked_data = marked_data + 1;
    set_name_safe(0x00007472, "COIN_CWS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007474)) {
    force_data_word(0x00007474);
    marked_data = marked_data + 1;
    set_name_safe(0x00007474, "COIN_SCI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007475)) {
    force_data_word(0x00007475);
    marked_data = marked_data + 1;
    set_name_safe(0x00007475, "COIN_MSG_NULLI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007476)) {
    force_data_word(0x00007476);
    marked_data = marked_data + 1;
    set_name_safe(0x00007476, "COIN_MSG_NULL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000747B)) {
    set_name_safe(0x0000747B, "COIN_FONT18REDDS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007480)) {
    force_data_word(0x00007480);
    marked_data = marked_data + 1;
    set_name_safe(0x00007480, "COIN_INSERTCOINSI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007481)) {
    force_data_word(0x00007481);
    marked_data = marked_data + 1;
    set_name_safe(0x00007481, "COIN_INSERTCOINS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007485)) {
    force_data_word(0x00007485);
    marked_data = marked_data + 1;
    set_name_safe(0x00007485, "COIN_HITSTARTI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007486)) {
    force_data_word(0x00007486);
    marked_data = marked_data + 1;
    set_name_safe(0x00007486, "COIN_HITSTART");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007489)) {
    set_name_safe(0x00007489, "INSERT_COINS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074A1)) {
    set_name_safe(0x000074A1, "COIN_SHOW_COINAGE");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074AE)) {
    set_name_safe(0x000074AE, "COIN_NO_START");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074B0)) {
    set_name_safe(0x000074B0, "COIN_SHOW_CREDITS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074BA)) {
    set_name_safe(0x000074BA, "COIN_GODO_CREDITS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074C6)) {
    set_name_safe(0x000074C6, "COIN_DO_COINAGE");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074CA)) {
    set_name_safe(0x000074CA, "COIN_FLASH_INSERTCOINS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074D0)) {
    set_name_safe(0x000074D0, "COIN_SHOW_INSERTCOINS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074DC)) {
    set_name_safe(0x000074DC, "COIN_NO_INSERTCOINS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074E1)) {
    set_name_safe(0x000074E1, "COIN_FLASH_INSERTCOINSX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074E2)) {
    set_name_safe(0x000074E2, "COIN_FLASH_START");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074F6)) {
    set_name_safe(0x000074F6, "COIN_START_SKIP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074FB)) {
    set_name_safe(0x000074FB, "COIN_FLASH_STARTX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000074FC)) {
    set_name_safe(0x000074FC, "COIN_PRINT_COINAGE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007526)) {
    set_name_safe(0x00007526, "COIN_PRINT_COINAGEX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007527)) {
    set_name_safe(0x00007527, "COIN_GET_COINAGE_HIGHT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007532)) {
    set_name_safe(0x00007532, "COIN_GCHX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007536)) {
    set_name_safe(0x00007536, "COIN_WHITE10FNT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000753B)) {
    set_name_safe(0x0000753B, "COIN_PRINT_CREDITS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000756F)) {
    set_name_safe(0x0000756F, "COIN_FLASH_TO_START");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007577)) {
    set_name_safe(0x00007577, "COIN_NO_TOSTART");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007583)) {
    set_name_safe(0x00007583, "COIN_PRINT_TOSTART");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000758E)) {
    set_name_safe(0x0000758E, "COIN_FLASH_TOSTARTX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000758F)) {
    set_name_safe(0x0000758F, "COIN_TOSTART_STRING");
    named = named + 1;
  }
  if (in_mapped_memory(0x000075A6)) {
    set_name_safe(0x000075A6, "COIN_TOCONT_STRING");
    named = named + 1;
  }
  if (in_mapped_memory(0x000075BF)) {
    set_name_safe(0x000075BF, "VOLUME_DISPLAY");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007658)) {
    force_data_word(0x00007658);
    marked_data = marked_data + 1;
    set_name_safe(0x00007658, "COIN_MOTIONDIS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007659)) {
    set_name_safe(0x00007659, "COIN_MOTION_VALID");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007669)) {
    force_data_word(0x00007669);
    marked_data = marked_data + 1;
    set_name_safe(0x00007669, "SEND_BUFFER_AI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000766A)) {
    force_data_word(0x0000766A);
    marked_data = marked_data + 1;
    set_name_safe(0x0000766A, "COMMQ_TMP_BUFFI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000766B)) {
    set_name_safe(0x0000766B, "CLEAR_LINK");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000767D)) {
    set_name_safe(0x0000767D, "COMMQ_DECODE_BUFFER");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000768A)) {
    set_name_safe(0x0000768A, "COMMQ_DECODE_LP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007692)) {
    set_name_safe(0x00007692, "COMMQ_DECLPX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007694)) {
    set_name_safe(0x00007694, "COMMQ_ISDONE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007697)) {
    force_data_word(0x00007697);
    marked_data = marked_data + 1;
    set_name_safe(0x00007697, "COMMQ_DECODE_BLOCKI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007698)) {
    force_data_word(0x00007698);
    marked_data = marked_data + 1;
    set_name_safe(0x00007698, "COMMQ_DECODE_BLOCK");
    named = named + 1;
  }
  if (in_mapped_memory(0x000076BB)) {
    set_name_safe(0x000076BB, "COMMQ_PACKET_INIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000076C1)) {
    set_name_safe(0x000076C1, "COMMQ_READY_TO_SEND");
    named = named + 1;
  }
  if (in_mapped_memory(0x000076CA)) {
    set_name_safe(0x000076CA, "COMMQ_MESSAGE_ADD");
    named = named + 1;
  }
  if (in_mapped_memory(0x000076DB)) {
    set_name_safe(0x000076DB, "COMMQ_JJAG");
    named = named + 1;
  }
  if (in_mapped_memory(0x000076DC)) {
    set_name_safe(0x000076DC, "COMMQ_MESSADDX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000076E1)) {
    set_name_safe(0x000076E1, "COMMQ_MESSAGE_ADD_SB");
    named = named + 1;
  }
  if (in_mapped_memory(0x000076EE)) {
    set_name_safe(0x000076EE, "COMMQ_MASBX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007773)) {
    set_name_safe(0x00007773, "COMMQ_SEND_MODE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007969)) {
    force_data_word(0x00007969);
    marked_data = marked_data + 1;
    set_name_safe(0x00007969, "TEXT_TEXT_FREEI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000796A)) {
    force_data_word(0x0000796A);
    marked_data = marked_data + 1;
    set_name_safe(0x0000796A, "TEXT_TEXT_ACTIVEI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000796B)) {
    force_data_word(0x0000796B);
    marked_data = marked_data + 1;
    set_name_safe(0x0000796B, "TEXT_TEXT_LISTI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000796E)) {
    force_data_word(0x0000796E);
    marked_data = marked_data + 1;
    set_name_safe(0x0000796E, "TEXT_FONTDIGITLG_A");
    set_cmt(0x0000796E, "alts: TEXT_FONTDIGITSM_A", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x0000796F)) {
    force_data_word(0x0000796F);
    marked_data = marked_data + 1;
    set_name_safe(0x0000796F, "TEXT_FONT18_A");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007970)) {
    force_data_word(0x00007970);
    marked_data = marked_data + 1;
    set_name_safe(0x00007970, "TEXT_TEXTTABLEFONT18");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007971)) {
    force_data_word(0x00007971);
    marked_data = marked_data + 1;
    set_name_safe(0x00007971, "TEXT_FONT40_A");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007972)) {
    force_data_word(0x00007972);
    marked_data = marked_data + 1;
    set_name_safe(0x00007972, "TEXT_FONT10_A");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007973)) {
    set_name_safe(0x00007973, "TEXT_TEXT_INIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000797C)) {
    set_name_safe(0x0000797C, "TEXT_TEXT_ADDDS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000798B)) {
    set_name_safe(0x0000798B, "TEXT_TEXT_ADD");
    named = named + 1;
  }
  if (in_mapped_memory(0x000079A1)) {
    set_name_safe(0x000079A1, "TEXT_SETSMDIGITFONT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000079AB)) {
    set_name_safe(0x000079AB, "TEXT_SETSMDIGITFONTDS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000079C3)) {
    set_name_safe(0x000079C3, "TEXT_SETLGDIGITFONTDS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000079D1)) {
    force_data_word(0x000079D1);
    marked_data = marked_data + 1;
    set_name_safe(0x000079D1, "TEXT_FONTN43_A");
    named = named + 1;
  }
  if (in_mapped_memory(0x000079D2)) {
    set_name_safe(0x000079D2, "TEXT_SETN43FONT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000079F4)) {
    set_name_safe(0x000079F4, "TEXT_SET40FONTDS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A02)) {
    force_data_word(0x00007A02);
    marked_data = marked_data + 1;
    set_name_safe(0x00007A02, "TEXT_OGSMFONT_TABI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A0D)) {
    set_name_safe(0x00007A0D, "TEXT_SET12FONTDS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A1B)) {
    set_name_safe(0x00007A1B, "TEXT_SET18FONT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A25)) {
    set_name_safe(0x00007A25, "TEXT_SET18FONTDS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A4B)) {
    set_name_safe(0x00007A4B, "TEXT_STRLEN");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A53)) {
    set_name_safe(0x00007A53, "TEXT_STRLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A57)) {
    set_name_safe(0x00007A57, "TEXT_STLP2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A6C)) {
    set_name_safe(0x00007A6C, "TEXT_STRLENNCHAR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A6E)) {
    set_name_safe(0x00007A6E, "TEXT_STRLENX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A75)) {
    set_name_safe(0x00007A75, "TEXT_TEXT_OUTPUT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A7D)) {
    set_name_safe(0x00007A7D, "TEXT_TEXTLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A88)) {
    set_name_safe(0x00007A88, "TEXT_NO_CENTER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A8C)) {
    set_name_safe(0x00007A8C, "TEXT_TEXT_RET");
    set_cmt(0x00007A8C, "alts: TEXT_NO_RIGHT", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00007A8D)) {
    set_name_safe(0x00007A8D, "TEXT_OLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007AA2)) {
    set_name_safe(0x00007AA2, "TEXT_IBO1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007AD3)) {
    set_name_safe(0x00007AD3, "TEXT_OUCX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007ADB)) {
    set_name_safe(0x00007ADB, "TEXT_ISFROZEN");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007AE3)) {
    set_name_safe(0x00007AE3, "TEXT_DELLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007AF3)) {
    set_name_safe(0x00007AF3, "TEXT_NODELETE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007AF4)) {
    set_name_safe(0x00007AF4, "TEXT_NXTGRP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007AF6)) {
    set_name_safe(0x00007AF6, "TEXT_TXTOUT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007AFC)) {
    set_name_safe(0x00007AFC, "TEXT_STRCPY");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007B01)) {
    set_name_safe(0x00007B01, "TEXT_SCPLP0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007B06)) {
    set_name_safe(0x00007B06, "TEXT_REGPLP0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007B12)) {
    set_name_safe(0x00007B12, "TEXT_STRCAT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007B1D)) {
    set_name_safe(0x00007B1D, "TEXT_SCLP0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007B21)) {
    set_name_safe(0x00007B21, "TEXT_REGLP0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007B29)) {
    set_name_safe(0x00007B29, "TEXT_SCLP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007B2D)) {
    set_name_safe(0x00007B2D, "TEXT_REGLP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007B3D)) {
    set_name_safe(0x00007B3D, "TEXT_REGLP2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007F3F)) {
    set_name_safe(0x00007F3F, "COMM_INIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007F4F)) {
    force_data_word(0x00007F4F);
    marked_data = marked_data + 1;
    set_name_safe(0x00007F4F, "COMM_RECEIVE_BUFFERI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007F50)) {
    force_data_word(0x00007F50);
    marked_data = marked_data + 1;
    set_name_safe(0x00007F50, "COMM_COMM_IOI");
    set_cmt(0x00007F50, "alts: COMM_CTL", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00007F51)) {
    set_name_safe(0x00007F51, "COMM_ENABLE_INT2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007F5A)) {
    set_name_safe(0x00007F5A, "COMM_BABA");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007F86)) {
    set_name_safe(0x00007F86, "CLRONE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007F96)) {
    set_name_safe(0x00007F96, "COMM_ROUTINE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007FA1)) {
    set_name_safe(0x00007FA1, "COMM_COMM_HOLDFORA2D");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007FAD)) {
    set_name_safe(0x00007FAD, "COMM_CM1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007FC1)) {
    set_name_safe(0x00007FC1, "COMM_WTLPMI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007FC9)) {
    set_name_safe(0x00007FC9, "COMM_CM2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007FCE)) {
    set_name_safe(0x00007FCE, "COMM_CM3");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007FD6)) {
    set_name_safe(0x00007FD6, "COMM_WTLPMI2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00007FE5)) {
    set_name_safe(0x00007FE5, "COMM_WTLPMI3");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080A8)) {
    set_name_safe(0x000080A8, "COMM_COMM_MASTER_ERR_X");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080B3)) {
    set_name_safe(0x000080B3, "COMM_CMERROR1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080B4)) {
    set_name_safe(0x000080B4, "COMM_CMERROR2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080B5)) {
    set_name_safe(0x000080B5, "COMM_CMERROR3");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080B6)) {
    set_name_safe(0x000080B6, "COMM_CMERROR4");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080B7)) {
    set_name_safe(0x000080B7, "COMM_CMERROR5");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080B8)) {
    set_name_safe(0x000080B8, "COMM_CMERROR6");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080B9)) {
    set_name_safe(0x000080B9, "COMM_CMERROR7");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080BA)) {
    set_name_safe(0x000080BA, "COMM_CMERROR8");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080BB)) {
    set_name_safe(0x000080BB, "COMM_CMERROR9");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080BC)) {
    set_name_safe(0x000080BC, "COMM_CMERROR10");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080BD)) {
    set_name_safe(0x000080BD, "COMM_CMERRORL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080BE)) {
    set_name_safe(0x000080BE, "COMM_CMERRORDEAD");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080C1)) {
    set_name_safe(0x000080C1, "COMM_COMM_MASTER_ERROR");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080D5)) {
    set_name_safe(0x000080D5, "COMM_DO_SLAVE_SYNC");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080E2)) {
    set_name_safe(0x000080E2, "COMM_WTLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000080FA)) {
    set_name_safe(0x000080FA, "COMM_COMM_SLAVE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008102)) {
    set_name_safe(0x00008102, "COMM_CS1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008108)) {
    set_name_safe(0x00008108, "COMM_CS2");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000810F)) {
    set_name_safe(0x0000810F, "COMM_CS20");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008114)) {
    set_name_safe(0x00008114, "COMM_WTLPI");
    named = named + 1;
  }
  if (in_mapped_memory(0x000081E8)) {
    set_name_safe(0x000081E8, "COMM_CML1WT");
    named = named + 1;
  }
  if (in_mapped_memory(0x000081F2)) {
    set_name_safe(0x000081F2, "COMM_COMM_SLAVE_ERR_X");
    named = named + 1;
  }
  if (in_mapped_memory(0x000081FD)) {
    set_name_safe(0x000081FD, "COMM_CSERROR1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000081FE)) {
    set_name_safe(0x000081FE, "COMM_CSERROR2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000081FF)) {
    set_name_safe(0x000081FF, "COMM_CSERROR3");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008200)) {
    set_name_safe(0x00008200, "COMM_CSERROR4");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008201)) {
    set_name_safe(0x00008201, "COMM_CSERROR5");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008202)) {
    set_name_safe(0x00008202, "COMM_CSERROR6");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008203)) {
    set_name_safe(0x00008203, "COMM_CSERROR7");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008204)) {
    set_name_safe(0x00008204, "COMM_CSERROR8");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008205)) {
    set_name_safe(0x00008205, "COMM_CSERROR9");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008206)) {
    set_name_safe(0x00008206, "COMM_CSERROR11");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008207)) {
    set_name_safe(0x00008207, "COMM_CSERRORDEAD");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000820A)) {
    set_name_safe(0x0000820A, "COMM_COMM_SLAVE_ERROR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008212)) {
    set_name_safe(0x00008212, "INFIN_FIND_HIGHEST_ROADY");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000821F)) {
    set_name_safe(0x0000821F, "INFIN_FHRYLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000824D)) {
    set_name_safe(0x0000824D, "INFIN_NOCHANCE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008255)) {
    set_name_safe(0x00008255, "INFIN_FHRY_X");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008264)) {
    set_name_safe(0x00008264, "INFINITY");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008270)) {
    set_name_safe(0x00008270, "INFIN_INFF0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008275)) {
    set_name_safe(0x00008275, "INFIN_INFF1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000827B)) {
    set_name_safe(0x0000827B, "INFIN_OK23");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000827E)) {
    set_name_safe(0x0000827E, "INFIN_OK554");
    named = named + 1;
  }
  if (in_mapped_memory(0x000082A3)) {
    set_name_safe(0x000082A3, "INFIN_LOOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000082D0)) {
    set_name_safe(0x000082D0, "INFIN_II33");
    named = named + 1;
  }
  if (in_mapped_memory(0x000082D2)) {
    set_name_safe(0x000082D2, "INFIN_II");
    named = named + 1;
  }
  if (in_mapped_memory(0x000082D3)) {
    set_name_safe(0x000082D3, "INFIN_III44");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008306)) {
    set_name_safe(0x00008306, "INFIN_NOCLIPPING");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008324)) {
    set_name_safe(0x00008324, "INFIN_PLOT_CONSTANTS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000832C)) {
    set_name_safe(0x0000832C, "INFIN_LOOP1A");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000832D)) {
    set_name_safe(0x0000832D, "INFIN_LOOP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000083BB)) {
    force_data_word(0x000083BB);
    marked_data = marked_data + 1;
    set_name_safe(0x000083BB, "INFIN_INFIN_POLYGONSI");
    named = named + 1;
  }
  if (in_mapped_memory(0x000083D4)) {
    set_name_safe(0x000083D4, "INFIN_TRANS_PTS");
    set_cmt(0x000083D4, "alts: INFIN_BLUESKY", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x000083DC)) {
    set_name_safe(0x000083DC, "INFIN_LKJ2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000083F6)) {
    set_name_safe(0x000083F6, "INFIN_LKJ25");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008574)) {
    set_name_safe(0x00008574, "SORT_SMOKE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000857B)) {
    set_name_safe(0x0000857B, "SMOKE_SSLOOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008587)) {
    set_name_safe(0x00008587, "SMOKE_UNLINK");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008589)) {
    set_name_safe(0x00008589, "SMOKE_LINK");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000858F)) {
    set_name_safe(0x0000858F, "SMOKE_SSLOOPEND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008592)) {
    set_name_safe(0x00008592, "SMOKE_SORT_SMOKEX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000866E)) {
    set_name_safe(0x0000866E, "SMOKE_INIT_COLLA_OBJS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000866F)) {
    set_name_safe(0x0000866F, "SMOKE_ICO_LOOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000869F)) {
    set_name_safe(0x0000869F, "SMOKE_ICO_LOOPX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000086A3)) {
    set_name_safe(0x000086A3, "SMOKE_INIT_SPARK_KILL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000086E4)) {
    set_name_safe(0x000086E4, "IMPACT_SPARK");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008705)) {
    set_name_safe(0x00008705, "SMOKE_IMPACT_SPARKX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008709)) {
    set_name_safe(0x00008709, "SMOKE_TOO_MANY_SPARKS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000870C)) {
    set_name_safe(0x0000870C, "SMOKE_TMS_LP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008714)) {
    set_name_safe(0x00008714, "SMOKE_TMSLPE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000871A)) {
    set_name_safe(0x0000871A, "SMOKE_TMSXCC");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008730)) {
    force_data_word(0x00008730);
    marked_data = marked_data + 1;
    set_name_safe(0x00008730, "SNDTAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008E73)) {
    force_data_word(0x00008E73);
    marked_data = marked_data + 1;
    set_name_safe(0x00008E73, "UTIL_SCREEN0I");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008E74)) {
    force_data_word(0x00008E74);
    marked_data = marked_data + 1;
    set_name_safe(0x00008E74, "UTIL_SCREEN1I");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008E75)) {
    force_data_word(0x00008E75);
    marked_data = marked_data + 1;
    set_name_safe(0x00008E75, "UTIL_SCRSIZI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008E76)) {
    set_name_safe(0x00008E76, "SETPAGE0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008E8C)) {
    force_data_word(0x00008E8C);
    marked_data = marked_data + 1;
    set_name_safe(0x00008E8C, "UTIL_PAGEWORD");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008E8D)) {
    set_name_safe(0x00008E8D, "UTIL_P1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008EA1)) {
    set_name_safe(0x00008EA1, "CLRSCRN");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008EA5)) {
    set_name_safe(0x00008EA5, "CLRSCRN0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008EA7)) {
    set_name_safe(0x00008EA7, "UTIL_CLRSC00");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008EC0)) {
    set_name_safe(0x00008EC0, "SCREEN_FILL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008ECF)) {
    set_name_safe(0x00008ECF, "UTIL_CLRSCL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008EDF)) {
    set_name_safe(0x00008EDF, "RANDOM");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008EE6)) {
    set_name_safe(0x00008EE6, "UTIL_RND2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008EEA)) {
    set_name_safe(0x00008EEA, "FRAND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008EF9)) {
    set_name_safe(0x00008EF9, "SFRAND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008F03)) {
    set_name_safe(0x00008F03, "RANDU0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008F11)) {
    set_name_safe(0x00008F11, "RANDPER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008F18)) {
    set_name_safe(0x00008F18, "UTIL_RANDPX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008F19)) {
    set_name_safe(0x00008F19, "INIT_LINKED_LIST");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008F23)) {
    set_name_safe(0x00008F23, "UTIL_INIT_LL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008F29)) {
    set_name_safe(0x00008F29, "GET_LLIST");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008F36)) {
    set_name_safe(0x00008F36, "UTIL_GETLL_X");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008F39)) {
    set_name_safe(0x00008F39, "UTIL_GETLL_ERR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008F43)) {
    set_name_safe(0x00008F43, "UTIL_ALLOCLIST_X");
    named = named + 1;
  }
  if (in_mapped_memory(0x00008F45)) {
    set_name_safe(0x00008F45, "UTIL_ALLOCLIST_ISERROR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009018)) {
    set_name_safe(0x00009018, "DYNAOBJ_INIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009039)) {
    set_name_safe(0x00009039, "DELDYNA");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009041)) {
    set_name_safe(0x00009041, "CARB_INIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009047)) {
    set_name_safe(0x00009047, "UTIL_CARINTL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000906A)) {
    set_name_safe(0x0000906A, "PUSHALL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009084)) {
    set_name_safe(0x00009084, "POPALL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000090CE)) {
    force_data_word(0x000090CE);
    marked_data = marked_data + 1;
    set_name_safe(0x000090CE, "SNDTABI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009137)) {
    set_name_safe(0x00009137, "SET_MASTER_VOL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000913C)) {
    set_name_safe(0x0000913C, "SET_TRACK_VOL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009148)) {
    set_name_safe(0x00009148, "SND_JI1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009159)) {
    set_name_safe(0x00009159, "RESET_SNDBRD");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009180)) {
    set_name_safe(0x00009180, "SND_WAITIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000918E)) {
    set_name_safe(0x0000918E, "SILENT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000919D)) {
    force_data_word(0x0000919D);
    marked_data = marked_data + 1;
    set_name_safe(0x0000919D, "SNDSTRI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000919E)) {
    set_name_safe(0x0000919E, "CLRSNDDB");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091AE)) {
    set_name_safe(0x000091AE, "VOLSNDFX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091B0)) {
    set_name_safe(0x000091B0, "ONESNDFX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091B1)) {
    set_name_safe(0x000091B1, "SND_SNDFX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091CE)) {
    set_name_safe(0x000091CE, "SND_ONESNDFX2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091D7)) {
    set_name_safe(0x000091D7, "SND_NOVOL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091E0)) {
    set_name_safe(0x000091E0, "SND_SNDLDXFX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091E1)) {
    set_name_safe(0x000091E1, "SND_L88");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091E7)) {
    set_name_safe(0x000091E7, "SND_NOSOUND_FX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091E9)) {
    set_name_safe(0x000091E9, "KILLSNDFX");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091EF)) {
    set_name_safe(0x000091EF, "SND_KILSFX1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091F5)) {
    set_name_safe(0x000091F5, "SND_KILSFX2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000091F6)) {
    set_name_safe(0x000091F6, "SND_KILSFX3");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009208)) {
    set_name_safe(0x00009208, "ONESND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009222)) {
    set_name_safe(0x00009222, "SND_SNDLD1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000922A)) {
    set_name_safe(0x0000922A, "SND_NOTRESETVOL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009232)) {
    set_name_safe(0x00009232, "SND_SNDLDX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009239)) {
    set_name_safe(0x00009239, "SENDSND");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000923F)) {
    set_name_safe(0x0000923F, "SND_NIRM");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000926F)) {
    set_name_safe(0x0000926F, "SND_RESETMUNGE");
    named = named + 1;
  }
  if (in_mapped_memory(0x000092B9)) {
    set_name_safe(0x000092B9, "SND_NOT_F1");
    named = named + 1;
  }
  if (in_mapped_memory(0x000092C8)) {
    set_name_safe(0x000092C8, "SND_NOT_F4");
    named = named + 1;
  }
  if (in_mapped_memory(0x000092D2)) {
    set_name_safe(0x000092D2, "SND_NOT_F6");
    set_cmt(0x000092D2, "alts: SND_RESETMUNGE_X", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x000092D5)) {
    set_name_safe(0x000092D5, "SND_RESET_QUIET");
    named = named + 1;
  }
  if (in_mapped_memory(0x000092E4)) {
    set_name_safe(0x000092E4, "SND_SNDLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000092E9)) {
    set_name_safe(0x000092E9, "SND_SNDPLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000092EF)) {
    set_name_safe(0x000092EF, "SND_SNDUPD");
    named = named + 1;
  }
  if (in_mapped_memory(0x000092FA)) {
    set_name_safe(0x000092FA, "SND_JUSTGO");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009301)) {
    set_name_safe(0x00009301, "SND_SNDCLRE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009305)) {
    set_name_safe(0x00009305, "SND_SNDUPX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009307)) {
    set_name_safe(0x00009307, "WAVE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009328)) {
    set_name_safe(0x00009328, "WAVE_LD_RAM");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000935F)) {
    force_data_word(0x0000935F);
    marked_data = marked_data + 1;
    set_name_safe(0x0000935F, "WAVE__ATTR_WAVETABI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009367)) {
    force_data_word(0x00009367);
    marked_data = marked_data + 1;
    set_name_safe(0x00009367, "WAVE__ATTR_WAVETAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x000093BD)) {
    set_name_safe(0x000093BD, "WAVE_CREDITS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000093CB)) {
    set_name_safe(0x000093CB, "WAVE_BEGIN_GAME");
    named = named + 1;
  }
  if (in_mapped_memory(0x000093D2)) {
    set_name_safe(0x000093D2, "INIT_SYSTEM");
    named = named + 1;
  }
  if (in_mapped_memory(0x000093FC)) {
    set_name_safe(0x000093FC, "LOAD_FIXED_PALETTES");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009506)) {
    set_name_safe(0x00009506, "WAVE_LOAD_VARIOUS_PALETTES");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000952D)) {
    set_name_safe(0x0000952D, "_COSI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009530)) {
    set_name_safe(0x00009530, "_SINE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009531)) {
    set_name_safe(0x00009531, "MATH_SINE0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009535)) {
    set_name_safe(0x00009535, "MATH_NOTNEG");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009540)) {
    set_name_safe(0x00009540, "MATH_PERFECT2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009545)) {
    set_name_safe(0x00009545, "MATH_ALLOK");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009549)) {
    set_name_safe(0x00009549, "MATH_PERFECT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000954E)) {
    force_data_word(0x0000954E);
    marked_data = marked_data + 1;
    set_name_safe(0x0000954E, "MATH_FORMULA");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000954F)) {
    force_data_word(0x0000954F);
    marked_data = marked_data + 1;
    set_name_safe(0x0000954F, "MATH_SINTABLEI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009550)) {
    force_data_word(0x00009550);
    marked_data = marked_data + 1;
    set_name_safe(0x00009550, "HALFPII");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009551)) {
    force_data_word(0x00009551);
    marked_data = marked_data + 1;
    set_name_safe(0x00009551, "PII");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009554)) {
    force_data_word(0x00009554);
    marked_data = marked_data + 1;
    set_name_safe(0x00009554, "MATH_RADFORM");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009555)) {
    force_data_word(0x00009555);
    marked_data = marked_data + 1;
    set_name_safe(0x00009555, "MATH_RADFORMI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009556)) {
    set_name_safe(0x00009556, "NORMITS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000955E)) {
    set_name_safe(0x0000955E, "MATH_NMS1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009561)) {
    set_name_safe(0x00009561, "NORMIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009568)) {
    set_name_safe(0x00009568, "ARCTANF");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000957B)) {
    set_name_safe(0x0000957B, "MATH_ATF0");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000957D)) {
    set_name_safe(0x0000957D, "MATH_AT1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000958E)) {
    force_data_word(0x0000958E);
    marked_data = marked_data + 1;
    set_name_safe(0x0000958E, "MATH_ATTABV");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000958F)) {
    force_data_word(0x0000958F);
    marked_data = marked_data + 1;
    set_name_safe(0x0000958F, "MATH_OFFTABV");
    named = named + 1;
  }
  if (in_mapped_memory(0x000095D8)) {
    set_name_safe(0x000095D8, "FIND_YMATRIX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009625)) {
    set_name_safe(0x00009625, "INITMAT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009636)) {
    set_name_safe(0x00009636, "VECTLEN");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000963F)) {
    set_name_safe(0x0000963F, "CPYMAT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009662)) {
    set_name_safe(0x00009662, "CLR_VECTORA");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000966C)) {
    set_name_safe(0x0000966C, "MATRIX_MUL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009689)) {
    set_name_safe(0x00009689, "NORMALIZE");
    named = named + 1;
  }
  if (in_mapped_memory(0x000096B4)) {
    set_name_safe(0x000096B4, "GEN_NORMAL");
    named = named + 1;
  }
  if (in_mapped_memory(0x000096CD)) {
    set_name_safe(0x000096CD, "CONCATMATV");
    named = named + 1;
  }
  if (in_mapped_memory(0x000096E0)) {
    set_name_safe(0x000096E0, "MATH_INLP2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000096E2)) {
    set_name_safe(0x000096E2, "CONCATMAT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009702)) {
    set_name_safe(0x00009702, "DIST_PT2LINE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009717)) {
    set_name_safe(0x00009717, "GETLINE_EQ_2D");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009961)) {
    force_data_word(0x00009961);
    marked_data = marked_data + 1;
    set_name_safe(0x00009961, "CMOSI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000996A)) {
    set_name_safe(0x0000996A, "CMOS_CMOS_ON_C");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009970)) {
    set_name_safe(0x00009970, "CMOS_CMOS_OFF_C");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009976)) {
    set_name_safe(0x00009976, "CMOS_CMOS_WPON_C");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000997E)) {
    set_name_safe(0x0000997E, "CMOS_CMOS_WPOFF_C");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009987)) {
    set_name_safe(0x00009987, "CMOS_VALIDATE_CMOS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000999D)) {
    set_name_safe(0x0000999D, "CMOS_RESET_BOOKKEEPING");
    named = named + 1;
  }
  if (in_mapped_memory(0x000099A5)) {
    set_name_safe(0x000099A5, "CMOS_RBLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000099A9)) {
    set_name_safe(0x000099A9, "CMOS_VADJTAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x000099AC)) {
    set_name_safe(0x000099AC, "CMOS_VERIFY_ADJUSTMENTS_ACCURACY");
    named = named + 1;
  }
  if (in_mapped_memory(0x000099B6)) {
    set_name_safe(0x000099B6, "CMOS_VAALP");
    named = named + 1;
  }
  if (in_mapped_memory(0x000099BF)) {
    set_name_safe(0x000099BF, "CMOS_DORST");
    named = named + 1;
  }
  if (in_mapped_memory(0x000099C2)) {
    set_name_safe(0x000099C2, "CMOS_NORST");
    named = named + 1;
  }
  if (in_mapped_memory(0x000099CE)) {
    set_name_safe(0x000099CE, "CMOS_RESET_ADJUSTMENTS");
    named = named + 1;
  }
  if (in_mapped_memory(0x000099D7)) {
    set_name_safe(0x000099D7, "CMOS_VAALP2");
    named = named + 1;
  }
  if (in_mapped_memory(0x000099F5)) {
    set_name_safe(0x000099F5, "CMOS_CHECKSUMGEN_ADJ");
    named = named + 1;
  }
  if (in_mapped_memory(0x000099FA)) {
    set_name_safe(0x000099FA, "CMOS_VAALP3");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009A05)) {
    set_name_safe(0x00009A05, "CMOS_AUDIT_INC");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009A14)) {
    set_name_safe(0x00009A14, "CMOS_ADJUSTMENT_READ");
    set_cmt(0x00009A14, "alts: CMOS_AUDIT_READ", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00009A19)) {
    set_name_safe(0x00009A19, "CMOS_ADJUSTMENT_WRITE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009A1F)) {
    set_name_safe(0x00009A1F, "CMOS_AUDIT_WRITE");
    set_cmt(0x00009A1F, "alts: CMOS_AUDIT_WRITE_ADJ", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00009A24)) {
    set_name_safe(0x00009A24, "_rd_cw");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009A38)) {
    set_name_safe(0x00009A38, "_wr_cw");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009A9F)) {
    set_name_safe(0x00009A9F, "CMOS_TABLEENT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009AA2)) {
    set_name_safe(0x00009AA2, "CMOS_INIT_LASTHS_TABLE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009AC4)) {
    set_name_safe(0x00009AC4, "CMOS_INIT_HSTD_TABLES");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009AD4)) {
    set_name_safe(0x00009AD4, "CMOS_VALIDATE_HSTD_TABLES");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009AE1)) {
    set_name_safe(0x00009AE1, "CMOS_J1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009AE7)) {
    set_name_safe(0x00009AE7, "CMOS_J2");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009AED)) {
    set_name_safe(0x00009AED, "CMOS_J3");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009AFB)) {
    set_name_safe(0x00009AFB, "CMOS_RESETALL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009AFD)) {
    set_name_safe(0x00009AFD, "CMOS_GET_TABLE_ADDR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009B1A)) {
    set_name_safe(0x00009B1A, "CMOS_TEWL1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009B31)) {
    set_name_safe(0x00009B31, "CMOS_TABLE_ENTRY_READ");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D0B)) {
    force_data_word(0x00009D0B);
    marked_data = marked_data + 1;
    set_name_safe(0x00009D0B, "COUNTDOWN_BUFI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D0C)) {
    force_data_word(0x00009D0C);
    marked_data = marked_data + 1;
    set_name_safe(0x00009D0C, "HUD_MPH_BUFFERI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D0D)) {
    force_data_word(0x00009D0D);
    marked_data = marked_data + 1;
    set_name_safe(0x00009D0D, "HUD_STOPBUFFI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D2A)) {
    set_name_safe(0x00009D2A, "HUD");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D34)) {
    set_name_safe(0x00009D34, "HUD_SAFETS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D41)) {
    set_name_safe(0x00009D41, "HUD_NOMOTIONMSGS");
    set_cmt(0x00009D41, "alts: HUD_BLAHBLAH", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D57)) {
    set_name_safe(0x00009D57, "HUD_NOTLOST");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D63)) {
    set_name_safe(0x00009D63, "HUD_OFJN");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D70)) {
    set_name_safe(0x00009D70, "HUD_NOT_OFFROAD");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D80)) {
    set_name_safe(0x00009D80, "HUD_ISAUTOTRNS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D86)) {
    set_name_safe(0x00009D86, "HUD_DNDNL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009D91)) {
    set_name_safe(0x00009D91, "HUD_GEARS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009DA2)) {
    set_name_safe(0x00009DA2, "HUD_ISMPHT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009DA8)) {
    set_name_safe(0x00009DA8, "HUD_ISDNN");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009DCD)) {
    set_name_safe(0x00009DCD, "HUD_NOINITBP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009DD5)) {
    set_name_safe(0x00009DD5, "HUD_NBEEP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009DDD)) {
    set_name_safe(0x00009DDD, "HUD_ISMPH");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009DDE)) {
    set_name_safe(0x00009DDE, "HUD_ISKPH");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009E21)) {
    set_name_safe(0x00009E21, "HARDalloc_section");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009E4D)) {
    set_name_safe(0x00009E4D, "HUD_TACHOMETER_ANIMATE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009E52)) {
    set_name_safe(0x00009E52, "HUD_LP89");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009E84)) {
    set_name_safe(0x00009E84, "FILL_DITHER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009EAB)) {
    set_name_safe(0x00009EAB, "PALL_PAL_INIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009EF8)) {
    set_name_safe(0x00009EF8, "PALL_PAL_FIND");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F03)) {
    set_name_safe(0x00009F03, "PALL_FPLXEX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F06)) {
    set_name_safe(0x00009F06, "PALL_PAL_FIND_RAW");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F0A)) {
    set_name_safe(0x00009F0A, "PALL_FINDRLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F25)) {
    set_name_safe(0x00009F25, "PALL_PAL_ALLOC");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F31)) {
    set_name_safe(0x00009F31, "PALL_GPL0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F37)) {
    set_name_safe(0x00009F37, "PALL_GPLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F41)) {
    set_name_safe(0x00009F41, "PALL_GETPL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F52)) {
    set_name_safe(0x00009F52, "PALL_GPLX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F5A)) {
    set_name_safe(0x00009F5A, "PALL_PAL_ALLOC_RAW");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F65)) {
    set_name_safe(0x00009F65, "PALL_RPLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F6F)) {
    set_name_safe(0x00009F6F, "PALL_RAWPL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F7B)) {
    set_name_safe(0x00009F7B, "PALL_RPLX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F80)) {
    set_name_safe(0x00009F80, "PALL_RAWPEX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F87)) {
    set_name_safe(0x00009F87, "PALL_PAL_SET");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009F9D)) {
    set_name_safe(0x00009F9D, "PALL_PALXFER_INIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009FA8)) {
    set_name_safe(0x00009FA8, "PALL_PXIL");
    named = named + 1;
  }
  if (in_mapped_memory(0x00009FAF)) {
    set_name_safe(0x00009FAF, "PALL_PALXFER_GET");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A2D7)) {
    set_name_safe(0x0000A2D7, "COMP_BIT_ADDR");
    set_cmt(0x0000A2D7, "alts: COMP_CURRENT_CODE_BITS, COMP_NEXT_CODE, COMP_PUTC_BUF, COMP_PUTC_SH, COMP_bufcnt, COMP_character, COMP_count, COMP_new_code, COMP_old_code, COMP_retval", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A2D8)) {
    force_data_word(0x0000A2D8);
    marked_data = marked_data + 1;
    set_name_safe(0x0000A2D8, "COMP_DICTI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A2DB)) {
    set_name_safe(0x0000A2DB, "COMP_INPUT_BITS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A2E5)) {
    set_name_safe(0x0000A2E5, "COMP_MULTIWORD");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A2F4)) {
    set_name_safe(0x0000A2F4, "COMP_PUTC");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A316)) {
    set_name_safe(0x0000A316, "COMP_WVWRLP2");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A324)) {
    set_name_safe(0x0000A324, "DECOMPRESS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A340)) {
    set_name_safe(0x0000A340, "COMP_NOHARDLOAD");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A342)) {
    set_name_safe(0x0000A342, "COMP_DECOMPRESS_TOPLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A34B)) {
    set_name_safe(0x0000A34B, "COMP_CONT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A34E)) {
    set_name_safe(0x0000A34E, "DECOMPRESS_PROC");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A35F)) {
    set_name_safe(0x0000A35F, "COMP_DECOMPRESS_TOPLP3");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A36D)) {
    set_name_safe(0x0000A36D, "COMP_DECOMPRESSLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A380)) {
    set_name_safe(0x0000A380, "COMP_NODS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A381)) {
    set_name_safe(0x0000A381, "COMP_NODSRT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A383)) {
    set_name_safe(0x0000A383, "COMP_DECODEL3i");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A38A)) {
    set_name_safe(0x0000A38A, "COMP_DECODEL1i");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A392)) {
    set_name_safe(0x0000A392, "COMP_BLOOPER");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A399)) {
    set_name_safe(0x0000A399, "COMP_DECOMPRESSX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A39F)) {
    set_name_safe(0x0000A39F, "COMP_SAVE_DECOMP_REGS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A3B7)) {
    set_name_safe(0x0000A3B7, "COMP_RESTORE_DECOMP_REGS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A3CF)) {
    force_data_word(0x0000A3CF);
    marked_data = marked_data + 1;
    set_name_safe(0x0000A3CF, "BOOT_PACIFY_SCREEN_P");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A3D0)) {
    set_name_safe(0x0000A3D0, "COMP_BOOT_PACIFY_SCREEN");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A3E7)) {
    set_name_safe(0x0000A3E7, "COMP_LL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A3EB)) {
    set_name_safe(0x0000A3EB, "COMP_LLL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A3ED)) {
    set_name_safe(0x0000A3ED, "LOAD_SECTION_REQ");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A3FC)) {
    set_name_safe(0x0000A3FC, "COMP_NOWTLD");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A534)) {
    set_name_safe(0x0000A534, "DIV_F");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A55C)) {
    set_name_safe(0x0000A55C, "DIV_I30");
    set_cmt(0x0000A55C, "alts: DIV_I", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A573)) {
    set_name_safe(0x0000A573, "ROUTS_return");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A579)) {
    set_name_safe(0x0000A579, "ROUTS_div_32");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A57D)) {
    set_name_safe(0x0000A57D, "ROUTS_zero");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A57F)) {
    set_name_safe(0x0000A57F, "DIV_U30");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A596)) {
    set_name_safe(0x0000A596, "ROUTS_div_32b");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A5AC)) {
    set_name_safe(0x0000A5AC, "ROUTS_one");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A5AE)) {
    set_name_safe(0x0000A5AE, "ROUTS_zerob");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A5B0)) {
    set_name_safe(0x0000A5B0, "INV_F30");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A5D4)) {
    set_name_safe(0x0000A5D4, "MOD_I30");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A5EA)) {
    set_name_safe(0x0000A5EA, "ROUTS_returnc");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A5F0)) {
    set_name_safe(0x0000A5F0, "ROUTS_mod_32");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A5F4)) {
    set_name_safe(0x0000A5F4, "MOD_U30");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A60A)) {
    set_name_safe(0x0000A60A, "ROUTS_mod_32c");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A618)) {
    set_name_safe(0x0000A618, "ROUTS_onec");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A61C)) {
    set_name_safe(0x0000A61C, "SQRT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A723)) {
    set_name_safe(0x0000A723, "COMM_DRONE_PTR_SORT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A725)) {
    set_name_safe(0x0000A725, "COMMDRNE_REGULAR");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A779)) {
    set_name_safe(0x0000A779, "_itoaLZ");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A78D)) {
    set_name_safe(0x0000A78D, "_itoa");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A79E)) {
    set_name_safe(0x0000A79E, "FONT1A_itoa1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7AE)) {
    set_name_safe(0x0000A7AE, "FONT1A_NOLEADINGZERO");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7B4)) {
    set_name_safe(0x0000A7B4, "FONT1A_NOTNEG");
    set_cmt(0x0000A7B4, "alts: FONT1A_LOOP2", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7BD)) {
    set_name_safe(0x0000A7BD, "FONT1A_DALOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7C4)) {
    set_name_safe(0x0000A7C4, "FONT1A_itoaX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7CF)) {
    set_name_safe(0x0000A7CF, "FONT1A_ISZERO2");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7D7)) {
    set_name_safe(0x0000A7D7, "HEX2ASC");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7E4)) {
    set_name_safe(0x0000A7E4, "FONT1A_htoa1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7EA)) {
    set_name_safe(0x0000A7EA, "FONT1A_DDJ");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7EB)) {
    set_name_safe(0x0000A7EB, "FONT1A_JJK");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7F1)) {
    set_name_safe(0x0000A7F1, "FONT1A_LOOP2H");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A7FA)) {
    set_name_safe(0x0000A7FA, "FONT1A_HDALOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A801)) {
    set_name_safe(0x0000A801, "FONT1A_htoaX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A80C)) {
    set_name_safe(0x0000A80C, "FONT1A_ISZEROH");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A810)) {
    set_name_safe(0x0000A810, "_fill");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A811)) {
    set_name_safe(0x0000A811, "FONT1A_FILLLP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A81F)) {
    set_name_safe(0x0000A81F, "_outtextxyc");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A823)) {
    set_name_safe(0x0000A823, "FONT1A_OLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A833)) {
    set_name_safe(0x0000A833, "FONT1A_OUTRLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A836)) {
    set_name_safe(0x0000A836, "FONT1A_LOOP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A842)) {
    set_name_safe(0x0000A842, "FONT1A_NPLOT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A84A)) {
    set_name_safe(0x0000A84A, "TEXT_NXTCHAR");
    set_cmt(0x0000A84A, "alts: FONT1A_NXTCHAR", 0);
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A84C)) {
    set_name_safe(0x0000A84C, "FONT1A_oucX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A84F)) {
    set_name_safe(0x0000A84F, "_pixel");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A86B)) {
    force_data_word(0x0000A86B);
    marked_data = marked_data + 1;
    set_name_safe(0x0000A86B, "MPROC_PACTIVEI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A86C)) {
    force_data_word(0x0000A86C);
    marked_data = marked_data + 1;
    set_name_safe(0x0000A86C, "MPROC_PFREEI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A86D)) {
    force_data_word(0x0000A86D);
    marked_data = marked_data + 1;
    set_name_safe(0x0000A86D, "MPROC_PRCSTRI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A86F)) {
    set_name_safe(0x0000A86F, "MPROC_PRC_CREATE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A87C)) {
    set_name_safe(0x0000A87C, "MPROC_GETPROC0");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A891)) {
    set_name_safe(0x0000A891, "MPROC_GETPROCX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A893)) {
    set_name_safe(0x0000A893, "MPROC_PRC_CREATE_CHILD");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A89D)) {
    set_name_safe(0x0000A89D, "MPROC_PRC_DISPATCH");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A8AC)) {
    set_name_safe(0x0000A8AC, "MPROC_NEXTPRC");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A8AD)) {
    set_name_safe(0x0000A8AD, "MPROC_NP1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A8C0)) {
    set_name_safe(0x0000A8C0, "MPROC_DISPPRCX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A8C1)) {
    set_name_safe(0x0000A8C1, "MPROC_SUICIDE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A8C6)) {
    set_name_safe(0x0000A8C6, "MPROC_DIELP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A8D7)) {
    set_name_safe(0x0000A8D7, "MPROC_PRC_KILL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A8E4)) {
    set_name_safe(0x0000A8E4, "MPROC_KILLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A8F7)) {
    set_name_safe(0x0000A8F7, "MPROC_KILL_X");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A920)) {
    set_name_safe(0x0000A920, "MPROC_PRC_INIT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000A92C)) {
    set_name_safe(0x0000A92C, "MPROC_PINITL");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AB4A)) {
    set_name_safe(0x0000AB4A, "ERROR_ERROR_LOG");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AB99)) {
    set_name_safe(0x0000AB99, "ERROR_ERROR_X");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ABA9)) {
    set_name_safe(0x0000ABA9, "ERROR_ERROR_LOG_DISPLAY");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ABBC)) {
    set_name_safe(0x0000ABBC, "ERROR_ERRLOG_D");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ABC0)) {
    set_name_safe(0x0000ABC0, "ERROR_NEXT_PAGE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ABD1)) {
    set_name_safe(0x0000ABD1, "ERROR_ERRLOG_D2");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ABD4)) {
    set_name_safe(0x0000ABD4, "ERROR_BIGLOOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC01)) {
    set_name_safe(0x0000AC01, "ERROR_ION");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC08)) {
    set_name_safe(0x0000AC08, "ERROR_LLPPT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC17)) {
    set_name_safe(0x0000AC17, "ERROR_NOCLEAR");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC20)) {
    set_name_safe(0x0000AC20, "ERROR_NOT_NEXT_PAGE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC23)) {
    set_name_safe(0x0000AC23, "ERROR_BOOKX2");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC28)) {
    set_name_safe(0x0000AC28, "ERROR_BOOKX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC2C)) {
    set_name_safe(0x0000AC2C, "_rectangle");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC53)) {
    set_name_safe(0x0000AC53, "_line");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC7E)) {
    set_name_safe(0x0000AC7E, "LINE_L5");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC85)) {
    set_name_safe(0x0000AC85, "LINE_L44");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC8F)) {
    set_name_safe(0x0000AC8F, "LINE_L10");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC93)) {
    set_name_safe(0x0000AC93, "LINE_L43");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AC9D)) {
    set_name_safe(0x0000AC9D, "LINE_L14");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACA5)) {
    set_name_safe(0x0000ACA5, "LINE_L16");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACB2)) {
    set_name_safe(0x0000ACB2, "LINE_L17");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACB9)) {
    set_name_safe(0x0000ACB9, "LINE_L42");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACC3)) {
    set_name_safe(0x0000ACC3, "LINE_L22");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACC7)) {
    set_name_safe(0x0000ACC7, "LINE_L41");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACD1)) {
    set_name_safe(0x0000ACD1, "LINE_L26");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACD6)) {
    set_name_safe(0x0000ACD6, "LINE_L28");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACDA)) {
    set_name_safe(0x0000ACDA, "LINE_L40");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACE2)) {
    set_name_safe(0x0000ACE2, "LINE_L32");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACE6)) {
    set_name_safe(0x0000ACE6, "LINE_L34");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACEA)) {
    set_name_safe(0x0000ACEA, "LINE_L39");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000ACF1)) {
    set_name_safe(0x0000ACF1, "LINE_L38");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AE35)) {
    set_name_safe(0x0000AE35, "FLAMESORT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AE4E)) {
    set_name_safe(0x0000AE4E, "FLAME_FLMSORTX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AE57)) {
    set_name_safe(0x0000AE57, "FIND_NEXT_OBJ");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AE5B)) {
    set_name_safe(0x0000AE5B, "FLAME_OFN");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AEFC)) {
    set_name_safe(0x0000AEFC, "_rdma");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AF06)) {
    set_name_safe(0x0000AF06, "_stuff_fpga");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AF0E)) {
    set_name_safe(0x0000AF0E, "TOTALA_STUFFWT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AF43)) {
    set_name_safe(0x0000AF43, "BLTMOD2D");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AF78)) {
    set_name_safe(0x0000AF78, "BLTMOD2D_DS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AF84)) {
    force_data_word(0x0000AF84);
    marked_data = marked_data + 1;
    set_name_safe(0x0000AF84, "ROAD_DEBRISI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AF85)) {
    set_name_safe(0x0000AF85, "INIT_RDDEBRIS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AF88)) {
    set_name_safe(0x0000AF88, "ADD_RDDEBRIS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AF96)) {
    set_name_safe(0x0000AF96, "FREE_RDDEBRIS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AF9A)) {
    set_name_safe(0x0000AF9A, "ROADBLCK_FREELP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AFA4)) {
    force_data_word(0x0000AFA4);
    marked_data = marked_data + 1;
    set_name_safe(0x0000AFA4, "ROADBLCK_MAXDIST");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AFA5)) {
    set_name_safe(0x0000AFA5, "DEBRIS_SORT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AFBC)) {
    set_name_safe(0x0000AFBC, "ROADBLCK_DBINSLP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AFC2)) {
    set_name_safe(0x0000AFC2, "ROADBLCK_GETRK0");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AFD4)) {
    set_name_safe(0x0000AFD4, "ROADBLCK_GETLINK");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AFDD)) {
    set_name_safe(0x0000AFDD, "ROADBLCK_SHLOOP");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AFE8)) {
    set_name_safe(0x0000AFE8, "ROADBLCK_SHDONE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AFED)) {
    set_name_safe(0x0000AFED, "ROADBLCK_IDB1");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000AFF1)) {
    set_name_safe(0x0000AFF1, "ROADBLCK_IDBX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000B1BC)) {
    force_data_word(0x0000B1BC);
    marked_data = marked_data + 1;
    set_name_safe(0x0000B1BC, "HUD_SAFETSI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000B1C3)) {
    force_data_word(0x0000B1C3);
    marked_data = marked_data + 1;
    set_name_safe(0x0000B1C3, "HUD_GEARI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000B1D5)) {
    force_data_word(0x0000B1D5);
    marked_data = marked_data + 1;
    set_name_safe(0x0000B1D5, "POS_TABLEI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000B1D6)) {
    force_data_word(0x0000B1D6);
    marked_data = marked_data + 1;
    set_name_safe(0x0000B1D6, "HUD_POS_TABLE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000B2B3)) {
    force_data_word(0x0000B2B3);
    marked_data = marked_data + 1;
    set_name_safe(0x0000B2B3, "INVTAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C65B)) {
    force_data_word(0x0000C65B);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C65B, "ROADKILL_ROADKILL_TABI");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C65C)) {
    force_data_word(0x0000C65C);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C65C, "ROADKILL_SOUNDTIME");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C662)) {
    force_data_word(0x0000C662);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C662, "ROADKILL_SPINSPEEDF");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C6F0)) {
    force_data_word(0x0000C6F0);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C6F0, "SMOKE_SPARKANII");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C804)) {
    force_data_word(0x0000C804);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C804, "MOTION_LLG6X");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C805)) {
    force_data_word(0x0000C805);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C805, "MOTION_LLG6Y");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C806)) {
    force_data_word(0x0000C806);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C806, "MOTION_LLG6Z");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C91E)) {
    force_data_word(0x0000C91E);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C91E, "BSSSTART");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C977)) {
    force_data_word(0x0000C977);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C977, "CUSA_TIMERAM");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C9B0)) {
    force_data_word(0x0000C9B0);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C9B0, "MPROC_PACTIVE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C9B1)) {
    force_data_word(0x0000C9B1);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C9B1, "MPROC_PFREE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C9B2)) {
    force_data_word(0x0000C9B2);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C9B2, "OACTIVE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C9B3)) {
    force_data_word(0x0000C9B3);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C9B3, "OFREE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C9B5)) {
    force_data_word(0x0000C9B5);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C9B5, "OACTIVE_PRIORITY");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C9B6)) {
    force_data_word(0x0000C9B6);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C9B6, "OLOW_PRIORITY");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000C9B7)) {
    force_data_word(0x0000C9B7);
    marked_data = marked_data + 1;
    set_name_safe(0x0000C9B7, "DIRQ_OHIGH_PRIORITY");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E572)) {
    force_data_word(0x0000E572);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E572, "COIN_CUSTOM_COINTAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E58B)) {
    force_data_word(0x0000E58B);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E58B, "COIN_CREDITBUFFER");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E593)) {
    force_data_word(0x0000E593);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E593, "COIN_TOSTARTBUFFER");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E59B)) {
    force_data_word(0x0000E59B);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E59B, "COIN_SCS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E630)) {
    force_data_word(0x0000E630);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E630, "HUD_COUNTDOWN_BUF");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E632)) {
    force_data_word(0x0000E632);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E632, "HUD_MPH_BUFFER");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E639)) {
    force_data_word(0x0000E639);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E639, "HUD_STOPBUFFER");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E895)) {
    force_data_word(0x0000E895);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E895, "MAP_tmp_buffer");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E91C)) {
    force_data_word(0x0000E91C);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E91C, "ROAD_DEBRIS");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E920)) {
    force_data_word(0x0000E920);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E920, "SNDSTR");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E949)) {
    force_data_word(0x0000E949);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E949, "TEXT_TEXT_FREE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000E94A)) {
    force_data_word(0x0000E94A);
    marked_data = marked_data + 1;
    set_name_safe(0x0000E94A, "TEXT_TEXT_ACTIVE");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000EA7C)) {
    force_data_word(0x0000EA7C);
    marked_data = marked_data + 1;
    set_name_safe(0x0000EA7C, "_PALLIST");
    named = named + 1;
  }
  if (in_mapped_memory(0x0000EC17)) {
    force_data_word(0x0000EC17);
    marked_data = marked_data + 1;
    set_name_safe(0x0000EC17, "MPROC_PRCSTR");
    named = named + 1;
  }
  if (in_mapped_memory(0x00010585)) {
    force_data_word(0x00010585);
    marked_data = marked_data + 1;
    set_name_safe(0x00010585, "OBJ_OBJSTR");
    named = named + 1;
  }
  if (in_mapped_memory(0x0001979D)) {
    force_data_word(0x0001979D);
    marked_data = marked_data + 1;
    set_name_safe(0x0001979D, "COMP_DICT");
    named = named + 1;
  }
  if (in_mapped_memory(0x0001E598)) {
    force_data_word(0x0001E598);
    marked_data = marked_data + 1;
    set_name_safe(0x0001E598, "TEXT_TEXT_LIST");
    named = named + 1;
  }
  if (in_mapped_memory(0x0001F000)) {
    force_data_word(0x0001F000);
    marked_data = marked_data + 1;
    set_name_safe(0x0001F000, "SEND_BUFFER_A");
    named = named + 1;
  }
  if (in_mapped_memory(0x0001F258)) {
    force_data_word(0x0001F258);
    marked_data = marked_data + 1;
    set_name_safe(0x0001F258, "RECEIVE_BUFFER");
    named = named + 1;
  }
  if (in_mapped_memory(0x00809800)) {
    force_data_word(0x00809800);
    marked_data = marked_data + 1;
    set_name_safe(0x00809800, "_CAMERAPOS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00809809)) {
    force_data_word(0x00809809);
    marked_data = marked_data + 1;
    set_name_safe(0x00809809, "_CAMERAMATRIX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00809815)) {
    force_data_word(0x00809815);
    marked_data = marked_data + 1;
    set_name_safe(0x00809815, "TMPMAT");
    named = named + 1;
  }
  if (in_mapped_memory(0x00809818)) {
    force_data_word(0x00809818);
    marked_data = marked_data + 1;
    set_name_safe(0x00809818, "ROTATION_MATRIX");
    named = named + 1;
  }
  if (in_mapped_memory(0x0080982D)) {
    force_data_word(0x0080982D);
    marked_data = marked_data + 1;
    set_name_safe(0x0080982D, "DIRQ_POSTERMATRIX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00809889)) {
    force_data_word(0x00809889);
    marked_data = marked_data + 1;
    set_name_safe(0x00809889, "DIRQ_CLIPRAM");
    named = named + 1;
  }
  if (in_mapped_memory(0x008099DB)) {
    force_data_word(0x008099DB);
    marked_data = marked_data + 1;
    set_name_safe(0x008099DB, "DIRQ_POSTERMATRIX2D");
    named = named + 1;
  }
  if (in_mapped_memory(0x00809A1B)) {
    force_data_word(0x00809A1B);
    marked_data = marked_data + 1;
    set_name_safe(0x00809A1B, "COMMQ_COMMQ_TMP_BUFF");
    named = named + 1;
  }
  if (in_mapped_memory(0x00809D00)) {
    force_data_word(0x00809D00);
    marked_data = marked_data + 1;
    set_name_safe(0x00809D00, "BLOWLIST");
    named = named + 1;
  }
  if (in_mapped_memory(0x00900000)) {
    force_data_word(0x00900000);
    marked_data = marked_data + 1;
    set_name_safe(0x00900000, "UTIL_SCREEN0");
    named = named + 1;
  }
  if (in_mapped_memory(0x00940000)) {
    force_data_word(0x00940000);
    marked_data = marked_data + 1;
    set_name_safe(0x00940000, "UTIL_SCREEN1");
    named = named + 1;
  }
  if (in_mapped_memory(0x00997000)) {
    force_data_word(0x00997000);
    marked_data = marked_data + 1;
    set_name_safe(0x00997000, "COMM_COMM_IO");
    named = named + 1;
  }
  if (in_mapped_memory(0x009C0000)) {
    force_data_word(0x009C0000);
    marked_data = marked_data + 1;
    set_name_safe(0x009C0000, "CMOS_CMOS");
    named = named + 1;
  }
  if (in_mapped_memory(0x00C10DD2)) {
    force_data_word(0x00C10DD2);
    marked_data = marked_data + 1;
    set_name_safe(0x00C10DD2, "MATH_SINTABLE");
    named = named + 1;
  }
  if (in_mapped_memory(0x00C10ED3)) {
    force_data_word(0x00C10ED3);
    marked_data = marked_data + 1;
    set_name_safe(0x00C10ED3, "MATH_ATOFFTAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x00C10EE3)) {
    force_data_word(0x00C10EE3);
    marked_data = marked_data + 1;
    set_name_safe(0x00C10EE3, "MATH_ATTAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x00C10F64)) {
    force_data_word(0x00C10F64);
    marked_data = marked_data + 1;
    set_name_safe(0x00C10F64, "ROADKILL_ROADKILL_TAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x00C110BD)) {
    force_data_word(0x00C110BD);
    marked_data = marked_data + 1;
    set_name_safe(0x00C110BD, "SMOKE_SPARKANI");
    named = named + 1;
  }
  if (in_mapped_memory(0x00C111C0)) {
    force_data_word(0x00C111C0);
    marked_data = marked_data + 1;
    set_name_safe(0x00C111C0, "TEXTTAB_FONT18_TAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x00C1126C)) {
    force_data_word(0x00C1126C);
    marked_data = marked_data + 1;
    set_name_safe(0x00C1126C, "TEXTTAB_OGSMFONT_TAB");
    named = named + 1;
  }
  if (in_mapped_memory(0x00C12DF7)) {
    force_data_word(0x00C12DF7);
    marked_data = marked_data + 1;
    set_name_safe(0x00C12DF7, "MOTION_L4ABX");
    named = named + 1;
  }
  if (in_mapped_memory(0x00C12DFC)) {
    force_data_word(0x00C12DFC);
    marked_data = marked_data + 1;
    set_name_safe(0x00C12DFC, "MOTION_L4ABY");
    named = named + 1;
  }
  if (in_mapped_memory(0x00C12E01)) {
    force_data_word(0x00C12E01);
    marked_data = marked_data + 1;
    set_name_safe(0x00C12E01, "MOTION_L4ABZ");
    named = named + 1;
  }

  msg("[crusnusa] cleared=%d code_words=%d vectors=%d vec_funcs=%d dp0=%d labels=%d data=%d\n", cleared, code_words, vec_named, vec_funcs, dp_set, named, marked_data);
}

