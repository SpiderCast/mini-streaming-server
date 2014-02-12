* server-client 최초 접속 : client의 broadcasting

c://{
    "whereau": {
        "ip": "client IP"
        "ver": "client version"
    }
}
s://{
    "imhere": {
        "ip": "server IP"
        "ver": server version"
    }
}

* client가 현재 server의 상태 요구

c://{
    "your status": "channel" / "frontend"
}
s://{
    "channel": {
        "type": "TV" / "RADIO"
        "index": 채널 번호
        "name": "channel name"
        "provider": "provider name"
        "CAS": "CAS name" / "FTA"
        "searched TV": TV채널 갯수
        "searched RADIO": RADIO채널 갯수
    }
    "frontend": {
        "type": "DVB-S/S2" / "DVB-T/T2" / "DVB-C"
        "lock": True / False
        "frequency": frequency
        "symbol": symbolrate
        "polarity": "HOR" / "VER"
        "22kHz": True / False
        "QAM": 16 / 32 / 64 / 128 / 256
    }
}

* client가 server에게 동작 요청

c://{
    "channel change": {
        "type": "TV" / "RADIO"
        "index": 채널번호
    }
    "lock change": {
        "type": "DVB-S/S2" / "DVB-T/T2" / "DVB-C"
        "frequency": frequency
        "symbol": symbolrate
        "polarity": "HOR" / "VER"
        "22kHz": True / False
        "QAM": 16 / 32 / 64 / 128 / 256
    }
}
s://{
    "channel": { ... }
    "frontend": { ... }
}
