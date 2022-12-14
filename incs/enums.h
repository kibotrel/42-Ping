#pragma once

enum exit_codes {
  NO_ERROR,
  NO_ROOT_PERMISSION,
  DISPLAY_USAGE,
  FAILED_SOCKET_CREATION,
  SET_SOCKET_OPTIONS,
  MISSING_ADDRESS,
  UNKNOWN_ADDRESS,
  FAILED_SENT_PACKET,
  FAILED_GET_TIME,
  FAILED_SIGNAL_HOOK,
  FAILED_RECEIVE_PACKET,
  COUNT_NOT_POSITIVE,
  DELAY_TOO_SMALL,
  TTL_NOT_IN_RANGE,
};

enum boolean_values {
  FALSE,
  TRUE,
};

enum states {
  SUCCESS,
  FAIL,
};

enum error_types {
  REGULAR_ERROR,
  INTERNAL_ERROR,
};

enum flags {
  VERBOSE = 1,
  COUNT = 2,
  INTERVAL = 4,
  QUIET = 8,
  TIMESTAMP = 16,
  TTL = 32,
};
