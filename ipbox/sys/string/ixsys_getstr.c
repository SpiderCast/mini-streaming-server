/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsys_getstr.h"
#include "ixsys_print.h"
#include "ixsys_getstr_table.h"

/* ****************************************************************************************************************** */

char* ixsys_getstr_fetype(fe_type_t type)
{
    char    *str = NULL;

    switch (type) {
    case FE_QPSK:   str = FETYPE_STR_DVBS;      break;
    case FE_QAM:    str = FETYPE_STR_DVBC;      break;
    case FE_OFDM:   str = FETYPE_STR_DVBT;      break;
    case FE_ATSC:   str = FETYPE_STR_ATSC;      break;
    }

    return str;
}

char* ixsys_getstr_delivery_system(fe_delivery_system_t system)
{
    char    *str = NULL;

    switch (system) {
    case SYS_UNDEFINED:         str = SYSTEM_STR_UNDEFINED;         break;
    case SYS_DVBC_ANNEX_AC:     str = SYSTEM_STR_DVBC_ANNEX_AC;     break;
    case SYS_DVBC_ANNEX_B:      str = SYSTEM_STR_DVBC_ANNEX_B;      break;
    case SYS_DVBT:              str = SYSTEM_STR_DVBT;              break;
    case SYS_DSS:               str = SYSTEM_STR_DSS;               break;
    case SYS_DVBS:              str = SYSTEM_STR_DVBS;              break;
    case SYS_DVBS2:             str = SYSTEM_STR_DVBS2;             break;
    case SYS_DVBH:              str = SYSTEM_STR_DVBH;              break;
    case SYS_ISDBT:             str = SYSTEM_STR_ISDBT;             break;
    case SYS_ISDBS:             str = SYSTEM_STR_ISDBS;             break;
    case SYS_ISDBC:             str = SYSTEM_STR_ISDBC;             break;
    case SYS_ATSC:              str = SYSTEM_STR_ATSC;              break;
    case SYS_ATSCMH:            str = SYSTEM_STR_ATSCMH;            break;
    case SYS_DMBTH:             str = SYSTEM_STR_DMBTH;             break;
    case SYS_CMMB:              str = SYSTEM_STR_CMMB;              break;
    case SYS_DAB:               str = SYSTEM_STR_DAB;               break;
    case SYS_DVBT2:             str = SYSTEM_STR_DVBT2;             break;
    }

    return str;
}

char* ixsys_getstr_modulation(fe_modulation_t modulation)
{
    char    *str = NULL;

    switch (modulation) {
    case QPSK:          str = MODULATION_STR_QPSK;      break;
    case QAM_16:        str = MODULATION_STR_16QAM;     break;
    case QAM_32:        str = MODULATION_STR_32QAM;     break;
    case QAM_64:        str = MODULATION_STR_64QAM;     break;
    case QAM_128:       str = MODULATION_STR_128QAM;    break;
    case QAM_256:       str = MODULATION_STR_256QAM;    break;
    case QAM_AUTO:      str = MODULATION_STR_QAM_AUTO;  break;
    case VSB_8:         str = MODULATION_STR_8VSB;      break;
    case VSB_16:        str = MODULATION_STR_16VSB;     break;
    case PSK_8:         str = MODULATION_STR_8PSK;      break;
    case APSK_16:       str = MODULATION_STR_16APSK;    break;
    case APSK_32:       str = MODULATION_STR_32APSK;    break;
    case DQPSK:         str = MODULATION_STR_DQPSK;     break;
    }

    return str;
}

char* ixsys_getstr_lnb(ix_lnb_t lnb)
{
    char    *lnbstr = NULL;

    switch (lnb) {
    default:
    case _lnb_universal:    lnbstr = LNB_STR_UNIVERSAL;     break;
    case _lnb_single:       lnbstr = LNB_STR_SINGLE;        break;
    case _lnb_user:         lnbstr = LNB_STR_USER;          break;
    }

    return lnbstr;
}

char* ixsys_getstr_lnbp(ix_lnbp_t lnbp)
{
    char    *lnbpstr = NULL;

    switch (lnbp) {
    default:
    case _lnbp_on:      lnbpstr = LNBP_STR_ON;      break;
    case _lnbp_13v:     lnbpstr = LNBP_STR_13V;     break;
    case _lnbp_18v:     lnbpstr = LNBP_STR_18V;     break;
    case _lnbp_off:     lnbpstr = LNBP_STR_OFF;     break;
    }

    return lnbpstr;
}

char* ixsys_getstr_22khz(ix_22khz_t _22khz)
{
    char    *_22khzstr = NULL;

    switch (_22khz) {
    default:
    case _22khz_auto:   _22khzstr = _22KHZ_STR_AUTO;    break;
    case _22khz_on:     _22khzstr = _22KHZ_STR_ON;      break;
    case _22khz_off:    _22khzstr = _22KHZ_STR_OFF;     break;
    }

    return _22khzstr;
}

char* ixsys_getstr_diseqc(ix_diseqc_t diseqc)
{
    char    *diseqcstr = NULL;

    switch (diseqc) {
    default:
    case _diseqc_none:      diseqcstr = DISEQC_STR_NONE;    break;
    case _diseqc_10:        diseqcstr = DISEQC_STR_10;      break;
    case _diseqc_11:        diseqcstr = DISEQC_STR_11;      break;
    case _diseqc_motor:     diseqcstr = DISEQC_STR_MOTOR;   break;
    }

    return diseqcstr;
}

char* ixsys_getstr_polar(ix_polar_t polar)
{
    char    *polarstr = NULL;

    switch (polar) {
    default:
    case _polar_hor:    polarstr = POLAR_STR_HOR;   break;
    case _polar_ver:    polarstr = POLAR_STR_VER;   break;
    }

    return polarstr;
}

char* ixsys_getstr_streamingtype(ix_streaming_t type)
{
    char    *str = NULL;

    switch (type) {
    default:
    case _streaming_http:       str = STREAMING_TYPE_HTTP;          break;
    case _streaming_udp:        str = STREAMING_TYPE_UDP;           break;
    case _streaming_multicast:  str = STREAMING_TYPE_MULTICAST;     break;
    }

    return str;
}

fe_type_t ixsys_get_fetype_str(char *str)
{
    if (strcasecmp(str, FETYPE_STR_DVBS) == 0)
        return FE_QPSK;
    else if (strcasecmp(str, FETYPE_STR_DVBC) == 0)
        return FE_QAM;
    else if (strcasecmp(str, FETYPE_STR_DVBT) == 0)
        return FE_OFDM;
    else if (strcasecmp(str, FETYPE_STR_ATSC) == 0)
        return FE_ATSC;

    return -1;
}

fe_delivery_system_t ixsys_get_delivery_system_str(char *str)
{
    if (strcasecmp(str, SYSTEM_STR_UNDEFINED) == 0)
        return SYS_UNDEFINED;
    else if (strcasecmp(str, SYSTEM_STR_DVBC_ANNEX_AC) == 0)
        return SYS_DVBC_ANNEX_AC;
    else if (strcasecmp(str, SYSTEM_STR_DVBC_ANNEX_B) == 0)
        return SYS_DVBC_ANNEX_B;
    else if (strcasecmp(str, SYSTEM_STR_DVBT) == 0)
        return SYS_DVBT;
    else if (strcasecmp(str, SYSTEM_STR_DSS) == 0)
        return SYS_DSS;
    else if (strcasecmp(str, SYSTEM_STR_DVBS) == 0)
        return SYS_DVBS;
    else if (strcasecmp(str, SYSTEM_STR_DVBS2) == 0)
        return SYS_DVBS2;
    else if (strcasecmp(str, SYSTEM_STR_DVBH) == 0)
        return SYS_DVBH;
    else if (strcasecmp(str, SYSTEM_STR_ISDBT) == 0)
        return SYS_ISDBT;
    else if (strcasecmp(str, SYSTEM_STR_ISDBS) == 0)
        return SYS_ISDBS;
    else if (strcasecmp(str, SYSTEM_STR_ISDBC) == 0)
        return SYS_ISDBC;
    else if (strcasecmp(str, SYSTEM_STR_ATSC) == 0)
        return SYS_ATSC;
    else if (strcasecmp(str, SYSTEM_STR_ATSCMH) == 0)
        return SYS_ATSCMH;
    else if (strcasecmp(str, SYSTEM_STR_DMBTH) == 0)
        return SYS_DMBTH;
    else if (strcasecmp(str, SYSTEM_STR_CMMB) == 0)
        return SYS_CMMB;
    else if (strcasecmp(str, SYSTEM_STR_DAB) == 0)
        return SYS_DAB;
    else if (strcasecmp(str, SYSTEM_STR_DVBT2) == 0)
        return SYS_DVBT2;
    return -1;
}

fe_modulation_t ixsys_get_modulation_str(char *str)
{
    if (strcasecmp(str, MODULATION_STR_QPSK) == 0)
        return QPSK;
    else if (strcasecmp(str, MODULATION_STR_16QAM) == 0)
        return QAM_16;
    else if (strcasecmp(str, MODULATION_STR_32QAM) == 0)
        return QAM_32;
    else if (strcasecmp(str, MODULATION_STR_64QAM) == 0)
        return QAM_64;
    else if (strcasecmp(str, MODULATION_STR_128QAM) == 0)
        return QAM_128;
    else if (strcasecmp(str, MODULATION_STR_256QAM) == 0)
        return QAM_256;
    else if (strcasecmp(str, MODULATION_STR_QAM_AUTO) == 0)
        return QAM_AUTO;
    else if (strcasecmp(str, MODULATION_STR_8VSB) == 0)
        return VSB_8;
    else if (strcasecmp(str, MODULATION_STR_16VSB) == 0)
        return VSB_16;
    else if (strcasecmp(str, MODULATION_STR_8PSK) == 0)
        return PSK_8;
    else if (strcasecmp(str, MODULATION_STR_16APSK) == 0)
        return APSK_16;
    else if (strcasecmp(str, MODULATION_STR_32APSK) == 0)
        return APSK_32;
    else if (strcasecmp(str, MODULATION_STR_DQPSK) == 0)
        return DQPSK;

    return -1;
}

ix_lnb_t ixsys_get_lnb_str(char *str)
{
    if (strcasecmp(str, LNB_STR_UNIVERSAL) == 0)
        return _lnb_universal;
    else if (strcasecmp(str, LNB_STR_SINGLE) == 0)
        return _lnb_single;
    else if (strcasecmp(str, LNB_STR_USER) == 0)
        return _lnb_user;

    return -1;
}

ix_lnbp_t ixsys_get_lnbp_str(char *str)
{
    if (strcasecmp(str, LNBP_STR_ON) == 0)
        return _lnbp_on;
    else if (strcasecmp(str, LNBP_STR_13V) == 0)
        return _lnbp_13v;
    else if (strcasecmp(str, LNBP_STR_18V) == 0)
        return _lnbp_18v;
    else if (strcasecmp(str, LNBP_STR_OFF) == 0)
        return _lnbp_off;

    return -1;
}

ix_22khz_t ixsys_get_22khz_str(char *str)
{
    if (strcasecmp(str, _22KHZ_STR_AUTO) == 0)
        return _22khz_auto;
    else if (strcasecmp(str, _22KHZ_STR_ON) == 0)
        return _22khz_on;
    else if (strcasecmp(str, _22KHZ_STR_OFF) == 0)
        return _22khz_off;

    return -1;
}

ix_diseqc_t ixsys_get_diseqc_str(char *str)
{
    if (strcasecmp(str, DISEQC_STR_NONE) == 0)
        return _diseqc_none;
    else if (strcasecmp(str, DISEQC_STR_10) == 0)
        return _diseqc_10;
    else if (strcasecmp(str, DISEQC_STR_11) == 0)
        return _diseqc_11;
    else if (strcasecmp(str, DISEQC_STR_MOTOR) == 0)
        return _diseqc_motor;

    return -1;
}

ix_polar_t ixsys_get_polar_str(char *str)
{
    if (strcasecmp(str, POLAR_STR_HOR) == 0)
        return _polar_hor;
    else if (strcasecmp(str, POLAR_STR_VER) == 0)
        return _polar_ver;

    return -1;
}

ix_streaming_t ixsys_get_streamingtype_str(char *str)
{
    if (strcasecmp(str, STREAMING_TYPE_HTTP) == 0)
        return _streaming_http;
    else if (strcasecmp(str, STREAMING_TYPE_UDP) == 0)
        return _streaming_udp;
    else if (strcasecmp(str, STREAMING_TYPE_MULTICAST) == 0)
        return _streaming_multicast;

    return -1;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
