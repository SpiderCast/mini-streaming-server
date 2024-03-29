/*
 * section and descriptor parser
 *
 * Copyright (C) 2005 Andrew de Quincey (adq_dvb@lidskialf.net)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef _UCSI_MPEG_TYPES_H
#define _UCSI_MPEG_TYPES_H 1

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Known stream types.
 */
enum {
	MPEG_STREAM_TYPE_ISO11172_VIDEO    = 0x01,
	MPEG_STREAM_TYPE_ISO13818_2_VIDEO    = 0x02,
	MPEG_STREAM_TYPE_ISO11172_AUDIO    = 0x03,
	MPEG_STREAM_TYPE_ISO13818_3_AUDIO    = 0x04,
	MPEG_STREAM_TYPE_ISO13818_1_PRIVATE_SECTIONS = 0x05,
	MPEG_STREAM_TYPE_ISO13818_1_PRIVATE_PES  = 0x06,
	MPEG_STREAM_TYPE_ISO13522_MHEG     = 0x07,
	MPEG_STREAM_TYPE_ISO13818_DSMCC    = 0x08,
	MPEG_STREAM_TYPE_ITUH222_1     = 0x09,
	MPEG_STREAM_TYPE_ISO13818_6_A    = 0x0a,
	MPEG_STREAM_TYPE_ISO13818_6_B    = 0x0b,
	MPEG_STREAM_TYPE_ISO13818_6_C    = 0x0c,
	MPEG_STREAM_TYPE_ISO13818_6_D    = 0x0d,
	MPEG_STREAM_TYPE_ISO13818_1_AUX    = 0x0e,
	MPEG_STREAM_TYPE_ISO13818_7_AUDIO_ADTS = 0x0f,
	MPEG_STREAM_TYPE_ISO14496_2_VISUAL   = 0x10,
	MPEG_STREAM_TYPE_ISO14496_3_AUDIO_LATM = 0x11,
	MPEG_STREAM_TYPE_ISO14496_1_PES    = 0x12,
	MPEG_STREAM_TYPE_ISO14496_1_SECTIONS   = 0x13,
	MPEG_STREAM_TYPE_ISO14496_6_SYNCDOWNLOAD = 0x14,
	MPEG_STREAM_TYPE_METADATA_PES    = 0x15,
	MPEG_STREAM_TYPE_METADATA_SECTIONS   = 0x16,
	MPEG_STREAM_TYPE_METADATA_DSMCC_DATA   = 0x17,
	MPEG_STREAM_TYPE_METADATA_DSMCC_OBJECT = 0x18,
	MPEG_STREAM_TYPE_METADATA_SYNCDOWNLOAD = 0x19,
	/* wuyong@jbsignal.com */
	MPEG_STREAM_TYPE_ISO13818_11_IPMP_STREAM = 0x1a,
	MPEG_STREAM_TYPE_ISO14496_10_VIDEO = 0x1b,
	MPEG_STREAM_TYPE_AVS_VIDEO = 0x42,
	MPEG_STREAM_TYPE_IPMP_STREAM = 0x7f,
};

/**
 * Metadata formats
 */
enum {
	MPEG_METADATA_FORMAT_ISO15938_1_TEM				= 0x01,
	MPEG_METADATA_FORMAT_ISO15938_1_BIM				= 0x02,
	MPEG_METADATA_FORMAT_METADATA_APPLICATION_FORMAT		= 0x3F,
	MPEG_METADATA_FORMAT_METADATA_APPLICATION_FORMAT_ID		= 0xFF,
};

/**
 * MPEG 4 audio profile and levels.
 */
enum {
	MPEG4_AUDIO_PROFILE_MAIN_LEVEL_1		= 0x10,
	MPEG4_AUDIO_PROFILE_MAIN_LEVEL_2		= 0x11,
	MPEG4_AUDIO_PROFILE_MAIN_LEVEL_3		= 0x12,
	MPEG4_AUDIO_PROFILE_MAIN_LEVEL_4		= 0x13,

	MPEG4_AUDIO_PROFILE_SCALABLE_LEVEL_1		= 0x18,
	MPEG4_AUDIO_PROFILE_SCALABLE_LEVEL_2		= 0x19,
	MPEG4_AUDIO_PROFILE_SCALABLE_LEVEL_3		= 0x1a,
	MPEG4_AUDIO_PROFILE_SCALABLE_LEVEL_4		= 0x1b,

	MPEG4_AUDIO_PROFILE_SPEECH_LEVEL_1		= 0x20,
	MPEG4_AUDIO_PROFILE_SPEECH_LEVEL_2		= 0x21,

	MPEG4_AUDIO_PROFILE_SYNTHESIS_LEVEL_1		= 0x28,
	MPEG4_AUDIO_PROFILE_SYNTHESIS_LEVEL_2		= 0x29,
	MPEG4_AUDIO_PROFILE_SYNTHESIS_LEVEL_3		= 0x2a,

	MPEG4_AUDIO_PROFILE_HQ_LEVEL_1			= 0x30,
	MPEG4_AUDIO_PROFILE_HQ_LEVEL_2			= 0x31,
	MPEG4_AUDIO_PROFILE_HQ_LEVEL_3			= 0x32,
	MPEG4_AUDIO_PROFILE_HQ_LEVEL_4			= 0x33,
	MPEG4_AUDIO_PROFILE_HQ_LEVEL_5			= 0x34,
	MPEG4_AUDIO_PROFILE_HQ_LEVEL_6			= 0x35,
	MPEG4_AUDIO_PROFILE_HQ_LEVEL_7			= 0x36,
	MPEG4_AUDIO_PROFILE_HQ_LEVEL_8			= 0x37,

	MPEG4_AUDIO_PROFILE_LOW_DELAY_LEVEL_1		= 0x38,
	MPEG4_AUDIO_PROFILE_LOW_DELAY_LEVEL_2		= 0x39,
	MPEG4_AUDIO_PROFILE_LOW_DELAY_LEVEL_3		= 0x3a,
	MPEG4_AUDIO_PROFILE_LOW_DELAY_LEVEL_4		= 0x3b,
	MPEG4_AUDIO_PROFILE_LOW_DELAY_LEVEL_5		= 0x3c,
	MPEG4_AUDIO_PROFILE_LOW_DELAY_LEVEL_6		= 0x3d,
	MPEG4_AUDIO_PROFILE_LOW_DELAY_LEVEL_7		= 0x3e,
	MPEG4_AUDIO_PROFILE_LOW_DELAY_LEVEL_8		= 0x3f,

	MPEG4_AUDIO_PROFILE_NATURAL_LEVEL_1		= 0x40,
	MPEG4_AUDIO_PROFILE_NATURAL_LEVEL_2		= 0x41,
	MPEG4_AUDIO_PROFILE_NATURAL_LEVEL_3		= 0x42,
	MPEG4_AUDIO_PROFILE_NATURAL_LEVEL_4		= 0x43,

	MPEG4_AUDIO_PROFILE_MOBILE_LEVEL_1		= 0x48,
	MPEG4_AUDIO_PROFILE_MOBILE_LEVEL_2		= 0x49,
	MPEG4_AUDIO_PROFILE_MOBILE_LEVEL_3		= 0x4a,
	MPEG4_AUDIO_PROFILE_MOBILE_LEVEL_4		= 0x4b,
	MPEG4_AUDIO_PROFILE_MOBILE_LEVEL_5		= 0x4c,
	MPEG4_AUDIO_PROFILE_MOBILE_LEVEL_6		= 0x4d,
};


#ifdef __cplusplus
}
#endif

#endif
