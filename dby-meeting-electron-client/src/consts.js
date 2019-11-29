
export const IPC_EVENT = {
  BEFORE_QUIT: 'IPC_BEFORE_QUIT',
  APP_QUIT: 'IPC_APP_QUIT',
  DISABLE_QUIT: 'DISABLE_QUIT',
  TOGGLE_SHOW_DEBUG_INFO: 'TOGGLE_SHOW_DEBUG_INFO',
}

export const SDK_LINES = {
  DBY: 'DBY',
  AGORA: 'AGORA'
}


export const CAMERA_OUTPUT_OPTIONS = [
  {
    width: 320,
    height: 240,
  },
  {
    width: 640,
    height: 480,
  },
  {
    width: 1280,
    height: 720,
  },
]

export const NETWORK_QUALITY_TYPE = {
  /** 0: The network quality is unknown. */
  QUALITY_UNKNOWN: 0,
  /**  1: The network quality is excellent. */
  QUALITY_EXCELLENT: 1,
  /** 2: The network quality is quite good, but the bitrate may be slightly lower than excellent. */
  QUALITY_GOOD: 2,
  /** 3: Users can feel the communication slightly impaired. */
  QUALITY_POOR: 3,
  /** 4: Users cannot communicate smoothly. */
  QUALITY_BAD: 4,
  /** 5: The network is so bad that users can barely communicate. */
  QUALITY_VBAD: 5,
  /** 6: The network is down and users cannot communicate at all. */
  QUALITY_DOWN: 6,
  /** 7: Users cannot detect the network quality. (Not in use.) */
  QUALITY_UNSUPPORTED: 7,
  /** 8: Detecting the network quality. */
  QUALITY_DETECTING: 8,
};
