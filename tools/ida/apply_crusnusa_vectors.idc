#include <idc.idc>

static in_mapped_memory(ea)
{
  return get_segm_start(ea) != BADADDR;
}

static ensure_dword(ea)
{
  // On C31 in IDA, one address unit ("wide byte") is 32 bits.
  // Vector entries are one 32-bit word each, so size/stride is 1.
  del_items(ea, DELIT_SIMPLE, 1);
  create_byte(ea);
  return get_wide_byte(ea);
}

static set_name_safe(ea, name)
{
  set_name(ea, name, SN_FORCE);
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
  auto base, vector_count, entry_size;
  auto idx, vec_ea, target;
  auto name, ptr_name;
  auto named, created;

  base = 0x000000;
  vector_count = 0x40;
  entry_size = 1;
  named = 0;
  created = 0;

  for (idx = 0; idx < vector_count; idx = idx + 1)
  {
    vec_ea = base + idx * entry_size;
    target = bswap32(ensure_dword(vec_ea));

    name = vec_name(idx);
    set_name_safe(vec_ea, name);
    named = named + 1;

    ptr_name = form("%s_target_%08X", name, target);
    if (in_mapped_memory(target))
    {
      set_name_safe(target, ptr_name);
      if (maybe_make_func(target))
        created = created + 1;
    }
  }

  target = bswap32(get_wide_byte(base));
  maybe_make_func(target);

  msg("[crusnusa] Applied %d vectors, created/confirmed %d function targets, reset=0x%08X\n", named, created, target);
}
