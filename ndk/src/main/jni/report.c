#include "report.h"
#include "utils/string.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

int bsg_find_next_free_metadat_index(bugsnag_report *report) {
  int index;
  if (report->metadata.value_count < BUGSNAG_METADATA_MAX) {
    return report->metadata.value_count;
  } else {
    for (int i = 0; i < report->metadata.value_count; i++) {
      if (report->metadata.values[i].type == BSG_NONE_VALUE) {
        return i;
      }
    }
  }
  return -1;
}

int bugsnag_report_add_metadata_value(bugsnag_report *report, char *section,
                                      char *name) {
  int index = bsg_find_next_free_metadat_index(report);
  if (index < 0) {
    return index;
  }
  bsg_strncpy_safe(report->metadata.values[index].section, section,
                   sizeof(report->metadata.values[index].section));
  bsg_strncpy_safe(report->metadata.values[index].name, name,
                   sizeof(report->metadata.values[index].name));
  if (report->metadata.value_count < BUGSNAG_METADATA_MAX) {
    report->metadata.value_count = index + 1;
  }
  return index;
}
void bugsnag_report_add_metadata_double(bugsnag_report *report, char *section,
                                        char *name, double value) {
  int index = bugsnag_report_add_metadata_value(report, section, name);
  if (index > 0) {
    report->metadata.values[index].type = BSG_NUMBER_VALUE;
    report->metadata.values[index].double_value = value;
  }
}

void bugsnag_report_add_metadata_string(bugsnag_report *report, char *section,
                                        char *name, char *value) {
  int index = bugsnag_report_add_metadata_value(report, section, name);
  if (index > 0) {
    report->metadata.values[index].type = BSG_CHAR_VALUE;
    bsg_strncpy_safe(report->metadata.values[index].char_value, value,
                     sizeof(report->metadata.values[index].char_value));
  }
}

void bugsnag_report_add_metadata_bool(bugsnag_report *report, char *section,
                                      char *name, bool value) {
  int index = bugsnag_report_add_metadata_value(report, section, name);
  if (index > 0) {
    report->metadata.values[index].type = BSG_BOOL_VALUE;
    report->metadata.values[index].bool_value = value;
  }
}

void bugsnag_report_remove_metadata(bugsnag_report *report, char *section,
                                    char *name) {
  for (int i = 0; i < report->metadata.value_count; ++i) {
    if (strcmp(report->metadata.values[i].section, section) == 0 &&
        strcmp(report->metadata.values[i].name, name) == 0) {
      memcpy(&report->metadata.values[i],
             &report->metadata.values[report->metadata.value_count - 1],
             sizeof(bsg_metadata_value));
      report->metadata.values[report->metadata.value_count - 1].type =
          BSG_NONE_VALUE;
      report->metadata.value_count--;
      break;
    }
  }
}

void bugsnag_report_remove_metadata_tab(bugsnag_report *report, char *section) {
  for (int i = 0; i < report->metadata.value_count; ++i) {
    if (strcmp(report->metadata.values[i].section, section) == 0) {
      report->metadata.values[i].type = BSG_NONE_VALUE;
    }
  }
}

void bugsnag_report_start_session(bugsnag_report *report, char *session_id,
                                  long started_at) {
  bsg_strncpy_safe(report->session_id, session_id, sizeof(report->session_id));
  report->session_start = started_at;
  report->handled_events = 0;
}

void bugsnag_report_set_context(bugsnag_report *report, char *value) {
  bsg_strncpy_safe(report->context, value, sizeof(report->context));
}

void bugsnag_report_set_orientation(bugsnag_report *report, char *value) {
  bsg_strncpy_safe(report->device.orientation, value,
                   sizeof(report->device.orientation));
}

void bugsnag_report_set_build_uuid(bugsnag_report *report, char *value) {
  bsg_strncpy_safe(report->app.build_uuid, value,
                   sizeof(report->app.build_uuid));
}

void bugsnag_report_set_release_stage(bugsnag_report *report, char *value) {
  bsg_strncpy_safe(report->app.release_stage, value,
                   sizeof(report->app.release_stage));
}

void bugsnag_report_set_user_email(bugsnag_report *report, char *value) {
  bsg_strncpy_safe(report->user.email, value, sizeof(report->user.email));
}

void bugsnag_report_set_user_name(bugsnag_report *report, char *value) {
  bsg_strncpy_safe(report->user.name, value, sizeof(report->user.name));
}

void bugsnag_report_set_user_id(bugsnag_report *report, char *value) {
  bsg_strncpy_safe(report->user.id, value, sizeof(report->user.id));
}

void bugsnag_report_set_app_version(bugsnag_report *report, char *value) {
  bsg_strncpy_safe(report->app.version, value, sizeof(report->app.version));
}

void bugsnag_report_add_breadcrumb(bugsnag_report *report,
                                   bugsnag_breadcrumb *crumb) {
  int crumb_index;
  if (report->crumb_count < BUGSNAG_CRUMBS_MAX) {
    crumb_index = report->crumb_count;
    report->crumb_count++;
  } else {
    report->crumb_first_index++;
    if (report->crumb_first_index >= BUGSNAG_CRUMBS_MAX) {
      report->crumb_first_index = 0;
    }
    crumb_index = report->crumb_first_index - 1;
    if (crumb_index < 0) {
      crumb_index = BUGSNAG_CRUMBS_MAX - 1;
    }
  }
  memcpy(&report->breadcrumbs[crumb_index], crumb, sizeof(bugsnag_breadcrumb));
}

void bugsnag_report_clear_breadcrumbs(bugsnag_report *report) {
  report->crumb_count = 0;
  report->crumb_first_index = 0;
}
