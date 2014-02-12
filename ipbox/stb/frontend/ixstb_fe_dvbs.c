/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_fe_dvbs.h"
#include "ixsys_print.h"
#include <linux/dvb/frontend.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
#include "ixsys_db.h"
#include "ixsys_db_dvbs.h"

/* ****************************************************************************************************************** */

#define FE_PROPERTIES_LIMIT     10

/* ****************************************************************************************************************** */

static double get_usals_degree(int sat_latitude, int user_longitude, int user_latitude)
{
    double  sat_pos, longitude, latitude, result;
    double  pi = 3.1415926535;
    double  G23, G24, G25, G26, G27, G28, G29, G30, G31, G32, G36;
    double  tmp;

    sat_pos   = (double)sat_latitude / 10.0;                            // G8
    longitude = (double)user_longitude / 10.0;                          // G9
    latitude  = (double)user_latitude / 10.0;                           // G10

    if (fabs(sat_pos) > 180)    sat_pos   = 1000;                       // G14
    if (fabs(longitude) > 180)  longitude = 1000;                       // G15
    if (fabs(latitude) > 90)    latitude  = 1000;                       // G16

    if ((sat_pos > 999) || (longitude > 999) || (latitude > 999))
        sat_pos = 0;                                                    // G17

    if ((sat_pos > 999) || (longitude > 999) || (latitude > 999))
        longitude = 0;                                                  // G18

    if ((sat_pos > 999) || (longitude > 999) || (latitude > 999))
        latitude = 0;                                                   // G19

    result = fabs(sat_pos - longitude);                                 // G20

    if (result > 180)                                                   // G21
        result = -(360 - result);

    if (fabs(result) > 65)                                              // G22
        result = 0;

    G23 = 6378 * sin(pi * latitude / 180);                              // G23

    G24 = sqrt(40678884 - G23 * G23);                                   // G24

    G25 = G24 * sin(pi * result / 180);                                 // G25

    G26 = sqrt(G23 * G23 + G25 * G25);                                  // G26

    G27 = sqrt(40678884 - G26 * G26);                                   // G27

    G28 = 42164.2 - G27;                                                // G28

    G29 = sqrt(G28 * G28 + G26 * G26);                                  // G29

    G30 = sqrt((42164.2 - G24) * (42164.2 - G24) + (G23 * G23));        // G30

    G31 = sqrt(3555639523ul-3555639523ul * cos(pi * result/180));       // G31

    G32 = acos((G29 * G29 + G30 * G30 - G31 * G31) / (2 * G29 * G30));  // G32

    result = G32 * 180 / pi;                                            // G33

    if (fabs(result) > 65)                                              // G34
        result = 0;

    if (!(((sat_pos < longitude) && (latitude > 0)) || ((sat_pos > longitude) && (latitude < 0))))
        result = -(result);                                             // G35

    if ((sat_pos < -89.9) && (longitude > 89.9))
        G36 =  -(result);                                               // G36
    else
        G36 = result;

    if ((sat_pos > 89.9) && (longitude < -89.9))
        result =  -(result);                                            // G37
    else
        result = G36;

    /* if G33 is x.x5 then round up (x.x + 0.1) */
    tmp = fabs(result);
    tmp = floor(tmp * 10) / 10;

    if ((fabs(result) - tmp) >= 0.05)
        tmp += 0.1;

    if (result < 0)
        result = -(tmp);
    else
        result = tmp;

    return result;
}

static int set_lnbp(int fd, ix_dvbs_t *dvbs, ix_dvbs_tp_t *tp)
{
    fe_sec_voltage_t    voltage;

    switch (dvbs->lnbp) {
    case _lnbp_on:
        voltage = (tp->polar == _polar_ver) ? SEC_VOLTAGE_13 : SEC_VOLTAGE_18 ;
        break;
    case _lnbp_13v:
        voltage = SEC_VOLTAGE_13;
        break;
    case _lnbp_18v:
        voltage = SEC_VOLTAGE_18;
        break;
    default:
    case _lnbp_off:
        voltage = SEC_VOLTAGE_OFF;
        break;
    }

    if (ioctl(fd, FE_SET_VOLTAGE, voltage) == -1) {
        print_error("lnbp error");
        return -1;
    }

    return 0;
}

static int set_22khz(int fd, ix_22khz_t _22khz)
{
    if (ioctl(fd, FE_SET_TONE, (_22khz == _22khz_on) ? SEC_TONE_ON : SEC_TONE_OFF) == -1) {
        print_error("toneburst error");
        return -1;
    }

    return 0;
}

static int set_diseqc_10(int fd, ix_dvbs_t *dvbs, ix_dvbs_tp_t *tp)
{
    struct dvb_diseqc_master_cmd    diseqc = { { 0xe0, 0x10, 0x38, 0xf0, 0x00, 0x00 }, 4};
    ix_polar_t      polar;
    ix_22khz_t      _22khz;

    switch (dvbs->lnbp) {
    default:
    case _lnbp_on:
        polar = tp->polar;
        break;
    case _lnbp_13v:
        polar = _polar_ver;
        break;
    case _lnbp_18v:
        polar = _polar_hor;
        break;
    }

    switch (dvbs->_22khz) {
    default:
    case _22khz_auto:
        if ((dvbs->lnbf_ref > 0) && (tp->freq >= dvbs->lnbf_ref))
            _22khz = _22khz_on;
        else
            _22khz = _22khz_off;
        break;
    case _22khz_on:
        _22khz = _22khz_on;
        break;
    case _22khz_off:
        _22khz = _22khz_off;
        break;
    }

    diseqc.msg[3] |= (dvbs->diseqc_port - 1) << 2;

    if (polar == _polar_hor)
        diseqc.msg[3] |= 1 << 1;

    if (_22khz == _22khz_on)
        diseqc.msg[3] |= 1;

    if (ioctl(fd, FE_DISEQC_SEND_MASTER_CMD, &diseqc) == -1) {
        print_error("diseqc_10 error");
        return -1;
    }

    return 0;
}

static int set_diseqc_11(int fd, int port)
{
    struct dvb_diseqc_master_cmd    diseqc = { { 0xe0, 0x10, 0x39, 0xf0, 0x00, 0x00 }, 4};

    diseqc.msg[3] |= port;

    if (ioctl(fd, FE_DISEQC_SEND_MASTER_CMD, &diseqc) == -1) {
        print_error("diseqc_10 error");
        return -1;
    }

    return 0;
}

static int set_diseqc_motor(int fd, ix_dvbs_t *dvbs)
{
    ix_usals_cmd_t  cmd;
    int     param[3];
    int     n_param;

    if (dvbs->diseqc_port == 0) {
        /* usals */
        cmd = _usals_auto;
        param[0] = dvbs->sat_pos;
        ixsys_get_user_position(&param[1], &param[2]);
        n_param = 3;
    } else {
        /* goto N */
        cmd = _usals_goto_n;
        param[0] = dvbs->diseqc_port;
        n_param = 1;
    }

    return ixstb_fe_dvbs_set_diseqc_motor(fd, cmd, param, n_param);
}

static int set_diseqc(int fd, ix_dvbs_t *dvbs, ix_dvbs_tp_t *tp)
{
    switch (dvbs->diseqc) {
    case _diseqc_10:
        set_diseqc_10(fd, dvbs, tp);
        break;
    case _diseqc_11:
        set_diseqc_11(fd, dvbs->diseqc_port);
        break;
    case _diseqc_motor:
        set_diseqc_motor(fd, dvbs);
        break;
    default:
        break;
    }

    return 0;
}

static ix_22khz_t _22khz_status(ix_22khz_t *_22khz)
{
    static ix_22khz_t   _last_22khz_ = _22khz_off;

    if (_22khz)
        _last_22khz_ = *_22khz;

    return _last_22khz_;
}

static int set_tune(int fd, ix_dvbs_t *dvbs, ix_dvbs_tp_t *dvbs_tp)
{
    struct dvb_frontend_parameters  tuneto;
    struct dvb_frontend_event       ev;
    struct dtv_properties   dtv_cmd;
    struct dtv_property     clear = { .cmd = DTV_CLEAR };
    struct dtv_property     *props;
    int     num = 0;

    switch (dvbs->lnb) {
    default:
    case _lnb_universal:
    case _lnb_single:
    case _lnb_user:
        if ((dvbs->lnbf_ref > 0) && (dvbs_tp->freq >= dvbs->lnbf_ref))
            tuneto.frequency = abs(dvbs->lnbf_hi - dvbs_tp->freq);
        else
            tuneto.frequency = abs(dvbs->lnbf_lo - dvbs_tp->freq);
        break;
    }

    tuneto.frequency *= 1000;
    tuneto.inversion = INVERSION_AUTO;
    tuneto.u.qpsk.symbol_rate = dvbs_tp->sym * 1000;
    tuneto.u.qpsk.fec_inner   = FEC_AUTO;

    print_debug("tuneto.frequency: %d\n", tuneto.frequency);
    print_debug("tuneto.u.qpsk.symbol_rate: %d\n", tuneto.u.qpsk.symbol_rate);

    /* discard  stale QPSK events */
    while (ioctl(fd, FE_GET_EVENT, &ev) != -1) ;

    // if (ioctl(fd, FE_SET_FRONTEND, &tuneto) == -1) {
    //     print_error("set_frontend error");
    //     return -1;
    // }

    dtv_cmd.num = 1;
    dtv_cmd.props = &clear;

    if (ioctl(fd, FE_SET_PROPERTY, &dtv_cmd) == -1) {
        print_error("FE_SET_PROPERTY [clear] error");
        return -1;
    }

    props = (struct dtv_property*)calloc(FE_PROPERTIES_LIMIT, sizeof(struct dtv_property));

    props[num].cmd = DTV_DELIVERY_SYSTEM;
    props[num].u.data = dvbs_tp->system;
    num++;
    props[num].cmd = DTV_MODULATION;
    props[num].u.data = dvbs_tp->modulation;
    num++;
    props[num].cmd = DTV_FREQUENCY;
    props[num].u.data = tuneto.frequency;
    num++;
    props[num].cmd = DTV_SYMBOL_RATE;
    props[num].u.data = tuneto.u.qpsk.symbol_rate;
    num++;
    props[num].cmd = DTV_INNER_FEC;
    props[num].u.data = tuneto.u.qpsk.fec_inner;
    num++;
    props[num].cmd = DTV_INVERSION;
    props[num].u.data = INVERSION_AUTO;
    num++;
    props[num].cmd = DTV_ROLLOFF;
    props[num].u.data = ROLLOFF_AUTO;
    num++;
    props[num].cmd = DTV_PILOT;
    props[num].u.data = PILOT_AUTO;
    num++;
    props[num].cmd = DTV_TUNE;
    num++;

    dtv_cmd.num = num;
    dtv_cmd.props = props;

    if (ioctl(fd, FE_SET_PROPERTY, &dtv_cmd) == -1) {
        print_error("FE_SET_PROPERTY error: %s", strerror(errno));
        free(props);
        return -1;
    }

    free(props);

    return 0;
}

int ixstb_fe_dvbs_locking(int fd, ix_dvbs_t *dvbs, ix_dvbs_tp_t *tp)
{
    ix_22khz_t  _22khz = _22khz_status(NULL);

    set_lnbp(fd, dvbs, tp);

    if ((dvbs->diseqc != _diseqc_none) && (_22khz != _22khz_off)) {
        set_22khz(fd, _22khz_off);
        usleep(15000);
    }

    set_diseqc(fd, dvbs, tp);

    switch (dvbs->_22khz) {
    default:
    case _22khz_auto:
        if ((dvbs->lnbf_ref > 0) && (tp->freq >= dvbs->lnbf_ref))
            _22khz = _22khz_on;
        else
            _22khz = _22khz_off;
        break;
    case _22khz_on:
        _22khz = _22khz_on;
        break;
    case _22khz_off:
        _22khz = _22khz_off;
        break;
    }

    if ((dvbs->diseqc != _diseqc_none) && (_22khz != _22khz_off)) {
        usleep(15000);
        set_22khz(fd, _22khz_on);
    }

    _22khz_status(&_22khz);

    return set_tune(fd, dvbs, tp);
}

int ixstb_fe_dvbs_unlocking(int fd)
{
    ix_22khz_t  _22khz = _22khz_off;

    _22khz_status(&_22khz);

    if (ioctl(fd, FE_SET_VOLTAGE, SEC_VOLTAGE_OFF) == -1) {
        print_error("lnbp error");
        return -1;
    }

    return 0;
}

int ixstb_fe_dvbs_set_diseqc_motor(int fd, ix_usals_cmd_t cmd, int *param, int n_param)
{
    struct dvb_diseqc_master_cmd    diseqc = { { 0xe0, 0x31, 0x6b, 0x00, 0x00, 0x00 }, 4 };
    int     pos, longitude, latitude;
    double  degree;

    switch (cmd) {
    case _usals_step:
        if (param[0] > 1800) {
            diseqc.msg[2] = 0x69;   /* drive west */
            diseqc.msg[3] = 0xff - (3600 - param[0]);
        } else {
            diseqc.msg[2] = 0x68;   /* drive east */
            diseqc.msg[3] = 0xff - param[0];
        }
        diseqc.msg_len = 4;
        break;
    case _usals_angle:
        diseqc.msg[2] = 0x6e;       /* drive to angular position */

        pos = (param[0] > 1800) ? -(3600 - param[0]) : param[0] ;
        pos /= 10;

        diseqc.msg[3] = (abs(pos) & 0xf0) >> 4;
        diseqc.msg[4] = (abs(pos) & 0x0f) << 4;
        diseqc.msg[3] |= (pos < 0) ? 0xe0 : 0xd0 ;
        diseqc.msg_len = 5;
        break;
    case _usals_goto_n:
        diseqc.msg[2] = 0x6b;       /* drive to N */
        diseqc.msg[3] = param[0];
        diseqc.msg_len = 4;
        break;
    case _usals_store_n:
        diseqc.msg[2] = 0x6a;       /* store N */
        diseqc.msg[3] = param[0];
        diseqc.msg_len = 4;
        break;
    case _usals_auto:
        pos = (param[0] > 1800) ? -(3600 - param[0]) : param[0] ;
        longitude = (param[1] > 1800) ? -(3600 - param[1]) : param[1] ;
        latitude  = (param[2] > 1800) ? -(3600 - param[2]) : param[2] ;

        diseqc.msg[2] = 0x6e;       /* drive to angular position */

        if ((degree = get_usals_degree(pos, longitude, latitude)) != 0) {
            double  d_xy, d_z;
            int     i_xy, i_z;

            d_z = modf(degree, &d_xy);
            i_xy = (int)(fabs(d_xy) + 0.5);
            i_z = (int)(fabs(d_z * 10.0) + 0.5);

            switch (i_z) {
            default:
            case 1:
            case 2:     i_z += 1;   break;
            case 3:
            case 4:     i_z += 2;   break;
            case 5:     i_z += 3;   break;
            case 6:
            case 7:     i_z += 4;   break;
            case 8:
            case 9:     i_z += 5;   break;
            }

            diseqc.msg[3] = (i_xy & 0xf0) >> 4;
            diseqc.msg[4] = (i_xy & 0x0f) << 4;
            diseqc.msg[3] |= (degree < 0) ? 0xe0 : 0xd0 ;
            diseqc.msg[4] |= i_z;
        } else {
            /* it does not mean - dummy */
            pos /= 10;

            diseqc.msg[3] = (abs(pos) & 0xf0) >> 4;
            diseqc.msg[4] = (abs(pos) & 0x0f) << 4;

            diseqc.msg[3] |= (pos < 0) ? 0xe0 : 0xd0 ;
        }
        diseqc.msg_len = 5;
        break;
    default:
    case _usals_stop:
        diseqc.msg[2] = 0x60;
        diseqc.msg_len = 3;
        break;
    }

    if (ioctl(fd, FE_DISEQC_SEND_MASTER_CMD, &diseqc) == -1) {
        print_error("diseqc_motor error: %d", errno);
        return -1;
    }

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
