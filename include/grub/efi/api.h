/* efi.h - declare EFI types and functions */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2006  Free Software Foundation, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef GRUB_EFI_API_HEADER
#define GRUB_EFI_API_HEADER	1

#include <grub/types.h>

/* For consistency and safety, we name the EFI-defined types differently.
   All names are transformed into lower case, _t appended, and
   grub_efi_ prepended.  */

/* Constants.  */
#define GRUB_EFI_EVT_TIMER				0x80000000
#define GRUB_EFI_EVT_RUNTIME				0x40000000
#define GRUB_EFI_EVT_RUNTIME_CONTEXT			0x20000000
#define GRUB_EFI_EVT_NOTIFY_WAIT			0x00000100
#define GRUB_EFI_EVT_NOTIFY_SIGNAL			0x00000200
#define GRUB_EFI_EVT_SIGNAL_EXIT_BOOT_SERVICES		0x00000201
#define GRUB_EFI_EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE	0x60000202

#define GRUB_EFI_TPL_APPLICATION	4
#define GRUB_EFI_TPL_CALLBACK		8
#define GRUB_EFI_TPL_NOTIFY		16
#define GRUB_EFI_TPL_HIGH_LEVEL		31

#define GRUB_EFI_MEMORY_UC	0x0000000000000001
#define GRUB_EFI_MEMORY_WC	0x0000000000000002
#define GRUB_EFI_MEMORY_WT	0x0000000000000004
#define GRUB_EFI_MEMORY_WB	0x0000000000000008
#define GRUB_EFI_MEMORY_UCE	0x0000000000000010
#define GRUB_EFI_MEMORY_WP	0x0000000000001000
#define GRUB_EFI_MEMORY_RP	0x0000000000002000
#define GRUB_EFI_MEMORY_XP	0x0000000000004000
#define GRUB_EFI_MEMORY_RUNTIME	0x8000000000000000

#define GRUB_EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL	0x00000001
#define GRUB_EFI_OPEN_PROTOCOL_GET_PROTOCOL		0x00000002
#define GRUB_EFI_OPEN_PROTOCOL_TEST_PROTOCOL		0x00000004
#define GRUB_EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER	0x00000008
#define GRUB_EFI_OPEN_PROTOCOL_BY_DRIVER		0x00000010
#define GRUB_EFI_OPEN_PROTOCOL_BY_EXCLUSIVE		0x00000020

#define GRUB_EFI_VARIABLE_NON_VOLATILE		0x0000000000000001
#define GRUB_EFI_VARIABLE_BOOTSERVICE_ACCESS	0x0000000000000002
#define GRUB_EFI_VARIABLE_RUNTIME_ACCESS	0x0000000000000004

#define GRUB_EFI_TIME_ADJUST_DAYLIGHT	0x01
#define GRUB_EFI_TIME_IN_DAYLIGHT	0x02

#define GRUB_EFI_UNSPECIFIED_TIMEZONE	0x07FF

#define GRUB_EFI_OPTIONAL_PTR	0x00000001

/* Enumerations.  */
enum grub_efi_timer_delay
  {
    GRUB_EFI_TIMER_CANCEL,
    GRUB_EFI_TIMER_PERIODIC,
    GRUB_EFI_TIMER_RELATIVE
  };
typedef enum grub_efi_timer_delay grub_efi_timer_delay_t;

enum grub_efi_allocate_type
  {
    GRUB_EFI_ALLOCATE_ANY_PAGES,
    GRUB_EFI_ALLOCATE_MAX_ADDRESS,
    GRUB_EFI_ALLOCATE_ADDRESS,
    GRUB_EFI_MAX_ALLOCATION_TYPE
  };
typedef enum grub_efi_allocate_type grub_efi_allocate_type_t;

enum grub_efi_memory_type
  {
    GRUB_EFI_RESERVED_MEMORY_TYPE,
    GRUB_EFI_LOADER_CODE,
    GRUB_EFI_LOADER_DATA,
    GRUB_EFI_BOOT_SERVICES_CODE,
    GRUB_EFI_BOOT_SERVICES_DATA,
    GRUB_EFI_RUNTIME_SERVICES_CODE,
    GRUB_EFI_RUNTIME_SERVICES_DATA,
    GRUB_EFI_CONVENTIONAL_MEMORY,
    GRUB_EFI_UNUSABLE_MEMORY,
    GRUB_EFI_ACPI_RECLAIM_MEMORY,
    GRUB_EFI_ACPI_MEMORY_NVS,
    GRUB_EFI_MEMORY_MAPPED_IO,
    GRUB_EFI_MEMORY_MAPPED_IO_PORT_SPACE,
    GRUB_EFI_PAL_CODE,
    GRUB_EFI_MAX_MEMORY_TYPE
  };
typedef enum grub_efi_memory_type grub_efi_memory_type_t;

enum grub_efi_interface_type
  {
    GRUB_EFI_NATIVE_INTERFACE
  };
typedef enum grub_efi_interface_type grub_efi_interface_type_t;

enum grub_efi_locate_search_type
  {
    GRUB_EFI_ALL_HANDLES,
    GRUB_EFI_BY_REGISTER_NOTIFY,
    GRUB_EFI_BY_PROTOCOL
  };
typedef enum grub_efi_locate_search_type grub_efi_locate_search_type_t;

enum grub_efi_reset_type
  {
    GRUB_EFI_RESET_COLD,
    GRUB_EFI_RESET_WARM,
    GRUB_EFI_RESET_SHUTDOWN
  };
typedef enum grub_efi_reset_type grub_efi_reset_type_t;

/* Types.  */
typedef char grub_efi_boolean_t;
typedef long grub_efi_intn_t;
typedef unsigned long grub_efi_uintn_t;
typedef grub_int8_t grub_efi_int8_t;
typedef grub_uint8_t grub_efi_uint8_t;
typedef grub_int16_t grub_efi_int16_t;
typedef grub_uint16_t grub_efi_uint16_t;
typedef grub_int32_t grub_efi_int32_t;
typedef grub_uint32_t grub_efi_uint32_t;
typedef grub_int64_t grub_efi_int64_t;
typedef grub_uint64_t grub_efi_uint64_t;
typedef grub_uint8_t grub_efi_char8_t;
typedef grub_uint16_t grub_efi_char16_t;

typedef grub_efi_intn_t grub_efi_status_t;

#define GRUB_EFI_ERROR_CODE(value)	\
  ((1 << (sizeof (grub_efi_status_t) * 8 - 1)) | (value))

#define GRUB_EFI_WARNING_CODE(value)	(value)

#define GRUB_EFI_SUCCESS		0

#define GRUB_EFI_LOAD_ERROR		GRUB_EFI_ERROR_CODE (1)
#define GRUB_EFI_INVALID_PARAMETER	GRUB_EFI_ERROR_CODE (2)
#define GRUB_EFI_UNSUPPORTED		GRUB_EFI_ERROR_CODE (3)
#define GRUB_EFI_BAD_BUFFER_SIZE	GRUB_EFI_ERROR_CODE (4)
#define GRUB_EFI_BUFFER_TOO_SMALL	GRUB_EFI_ERROR_CODE (5)
#define GRUB_EFI_NOT_READY		GRUB_EFI_ERROR_CODE (6)
#define GRUB_EFI_DEVICE_ERROR		GRUB_EFI_ERROR_CODE (7)
#define GRUB_EFI_WRITE_PROTECTED	GRUB_EFI_ERROR_CODE (8)
#define GRUB_EFI_OUT_OF_RESOURCES	GRUB_EFI_ERROR_CODE (9)
#define GRUB_EFI_VOLUME_CORRUPTED	GRUB_EFI_ERROR_CODE (10)
#define GRUB_EFI_VOLUME_FULL		GRUB_EFI_ERROR_CODE (11)
#define GRUB_EFI_NO_MEDIA		GRUB_EFI_ERROR_CODE (12)
#define GRUB_EFI_MEDIA_CHANGED		GRUB_EFI_ERROR_CODE (13)
#define GRUB_EFI_NOT_FOUND		GRUB_EFI_ERROR_CODE (14)
#define GRUB_EFI_ACCESS_DENIED		GRUB_EFI_ERROR_CODE (15)
#define GRUB_EFI_NO_RESPONSE		GRUB_EFI_ERROR_CODE (16)
#define GRUB_EFI_NO_MAPPING		GRUB_EFI_ERROR_CODE (17)
#define GRUB_EFI_TIMEOUT		GRUB_EFI_ERROR_CODE (18)
#define GRUB_EFI_NOT_STARTED		GRUB_EFI_ERROR_CODE (19)
#define GRUB_EFI_ALREADY_STARTED	GRUB_EFI_ERROR_CODE (20)
#define GRUB_EFI_ABORTED		GRUB_EFI_ERROR_CODE (21)
#define GRUB_EFI_ICMP_ERROR		GRUB_EFI_ERROR_CODE (22)
#define GRUB_EFI_TFTP_ERROR		GRUB_EFI_ERROR_CODE (23)
#define GRUB_EFI_PROTOCOL_ERROR		GRUB_EFI_ERROR_CODE (24)
#define GRUB_EFI_INCOMPATIBLE_VERSION	GRUB_EFI_ERROR_CODE (25)
#define GRUB_EFI_SECURITY_VIOLATION	GRUB_EFI_ERROR_CODE (26)
#define GRUB_EFI_CRC_ERROR		GRUB_EFI_ERROR_CODE (27)

#define GRUB_EFI_WARN_UNKNOWN_GLYPH	GRUB_EFI_WARNING_CODE (1)
#define GRUB_EFI_WARN_DELETE_FAILURE	GRUB_EFI_WARNING_CODE (2)
#define GRUB_EFI_WARN_WRITE_FAILURE	GRUB_EFI_WARNING_CODE (3)
#define GRUB_EFI_WARN_BUFFER_TOO_SMALL	GRUB_EFI_WARNING_CODE (4)

typedef void *grub_efi_handle_t;
typedef void *grub_efi_event_t;
typedef grub_efi_uint64_t grub_efi_lba_t;
typedef grub_efi_uintn_t grub_efi_tpl_t;
typedef grub_uint8_t grub_efi_mac_address_t[32];
typedef grub_uint8_t grub_efi_ipv4_address_t[4];
typedef grub_uint8_t grub_efi_ipv6_address_t[8];
typedef grub_uint8_t grub_efi_ip_address_t[8] __attribute__ ((aligned(4)));
typedef grub_efi_uint64_t grub_efi_physical_address_t;
typedef grub_efi_uint64_t grub_efi_virtual_address_t;

struct grub_efi_guid
{
  grub_uint32_t data1;
  grub_uint16_t data2;
  grub_uint16_t data3;
  grub_uint8_t data4;
  grub_uint8_t data5;
  grub_uint8_t data6;
  grub_uint8_t data7;
  grub_uint8_t data8;
  grub_uint8_t data9;
  grub_uint8_t data10;
  grub_uint8_t data11;
} __attribute__ ((aligned(8)));
typedef struct grub_efi_guid grub_efi_guid_t;

struct grub_efi_memory_descriptor
{
  grub_efi_uint32_t type;
  grub_efi_physical_address_t physical_start;
  grub_efi_virtual_address_t virtual_start;
  grub_efi_uint64_t num_pages;
  grub_efi_uint64_t attribute;
};
typedef struct grub_efi_memory_descriptor grub_efi_memory_descriptor_t;

struct grub_efi_device_path
{
  grub_efi_uint8_t type;
  grub_efi_uint8_t sub_type;
  grub_efi_uint8_t length[2];
};
typedef struct grub_efi_device_path grub_efi_device_path_t;
/* XXX EFI does not define EFI_DEVICE_PATH_PROTOCOL but uses it.
   It seems to be identical to EFI_DEVICE_PATH.  */
typedef struct grub_efi_device_path grub_efi_device_path_protocol_t;

struct grub_efi_open_protocol_information_entry
{
  grub_efi_handle_t agent_handle;
  grub_efi_handle_t controller_handle;
  grub_efi_uint32_t attributes;
  grub_efi_uint32_t open_count;
};
typedef struct grub_efi_open_protocol_information_entry grub_efi_open_protocol_information_entry_t;

struct grub_efi_time
{
  grub_efi_uint16_t year;
  grub_efi_uint8_t month;
  grub_efi_uint8_t day;
  grub_efi_uint8_t hour;
  grub_efi_uint8_t minute;
  grub_efi_uint8_t second;
  grub_efi_uint8_t pad1;
  grub_efi_uint32_t nanosecond;
  grub_efi_int16_t time_zone;
  grub_efi_uint8_t daylight;
  grub_efi_uint8_t pad2;
};
typedef struct grub_efi_time grub_efi_time_t;

struct grub_efi_time_capabilities
{
  grub_efi_uint32_t resolution;
  grub_efi_uint32_t accuracy;
  grub_efi_boolean_t sets_to_zero;
};
typedef struct grub_efi_time_capabilities grub_efi_time_capabilities_t;

struct grub_efi_input_key
{
  grub_efi_uint16_t scan_code;
  grub_efi_char16_t unicode_char;
};
typedef struct grub_efi_input_key grub_efi_input_key_t;

struct grub_efi_simple_text_output_mode
{
  grub_efi_int32_t max_mode;
  grub_efi_int32_t mode;
  grub_efi_int32_t attribute;
  grub_efi_int32_t cursor_column;
  grub_efi_int32_t cursor_row;
  grub_efi_boolean_t cursor_visible;
};
typedef struct grub_efi_simple_text_output_mode grub_efi_simple_text_output_mode_t;

/* Tables.  */
struct grub_efi_table_header
{
  grub_efi_uint64_t signature;
  grub_efi_uint32_t revision;
  grub_efi_uint32_t header_size;
  grub_efi_uint32_t crc32;
  grub_efi_uint32_t reserved;
};
typedef struct grub_efi_table_header grub_efi_table_header_t;

struct grub_efi_boot_services
{
  grub_efi_table_header_t hdr;
  
  grub_efi_tpl_t
  (*raise_tpl) (grub_efi_tpl_t new_tpl);
  
  void
  (*restore_tpl) (grub_efi_tpl_t old_tpl);
  
  grub_efi_status_t
  (*allocate_pages) (grub_efi_allocate_type_t type,
		     grub_efi_memory_type_t memory_type,
		     grub_efi_uintn_t pages,
		     grub_efi_physical_address_t *memory);
  
  grub_efi_status_t
  (*free_pages) (grub_efi_physical_address_t memory,
		 grub_efi_uintn_t pages);
  
  grub_efi_status_t
  (*get_memory_map) (grub_efi_uintn_t *memory_map_size,
		     grub_efi_memory_descriptor_t *memory_map,
		     grub_efi_uintn_t *map_key,
		     grub_efi_uintn_t *descriptor_size,
		     grub_efi_uint32_t *descriptor_version);
  
  grub_efi_status_t
  (*allocate_pool) (grub_efi_memory_type_t pool_type,
		    grub_efi_uintn_t size,
		    void **buffer);
  
  grub_efi_status_t
  (*free_pool) (void *buffer);
  
  grub_efi_status_t
  (*create_event) (grub_efi_uint32_t type,
		   grub_efi_tpl_t notify_tpl,
		   void (*notify_function) (grub_efi_event_t event,
					    void *context),
		   void *notify_context,
		   grub_efi_event_t *event);

  grub_efi_status_t

  (*set_timer) (grub_efi_event_t event,
		grub_efi_timer_delay_t type,
		grub_efi_uint64_t trigger_time);
  
   grub_efi_status_t
   (*wait_for_event) (grub_efi_uintn_t num_events,
		      grub_efi_event_t *event,
		      grub_efi_uintn_t *index);

  grub_efi_status_t
  (*signal_event) (grub_efi_event_t event);
  
  grub_efi_status_t
  (*close_event) (grub_efi_event_t event);
  
  grub_efi_status_t
  (*check_event) (grub_efi_event_t event);

   grub_efi_status_t
   (*install_protocol_interface) (grub_efi_handle_t *handle,
				  grub_efi_guid_t *protocol,
				  grub_efi_interface_type_t interface_type,
				  void *interface);
  
  grub_efi_status_t
  (*reinstall_protocol_interface) (grub_efi_handle_t handle,
				   grub_efi_guid_t *protocol,
				   void *old_interface,
				   void *new_inteface);
  
  grub_efi_status_t
  (*uninstall_protocol_interface) (grub_efi_handle_t handle,
				   grub_efi_guid_t *protocol,
				   void *interface);

  grub_efi_status_t
  (*handle_protocol) (grub_efi_handle_t handle,
		      grub_efi_guid_t *protocol,
		      void **interface);
  
  void *reserved;
  
  grub_efi_status_t
  (*register_protocol_notify) (grub_efi_guid_t *protocol,
			       grub_efi_event_t event,
			       void **registration);

  grub_efi_status_t
  (*locate_handle) (grub_efi_locate_search_type_t search_type,
		    grub_efi_guid_t *protocol,
		    void *search_key,
		    grub_efi_uintn_t *buffer_size,
		    grub_efi_handle_t *buffer);

  grub_efi_status_t
  (*locate_device_path) (grub_efi_guid_t *protocol,
			 grub_efi_device_path_t **device_path,
			 grub_efi_handle_t *device);

  grub_efi_status_t
  (*install_configuration_table) (grub_efi_guid_t *guid, void *table);

  grub_efi_status_t
  (*load_image) (grub_efi_boolean_t boot_policy,
		 grub_efi_handle_t parent_image_handle,
		 grub_efi_device_path_t *file_path,
		 void *source_buffer,
		 grub_efi_uintn_t source_size,
		 grub_efi_handle_t *image_handle);

  grub_efi_status_t
  (*start_image) (grub_efi_handle_t image_handle,
		  grub_efi_uintn_t *exit_data_size,
		  grub_efi_char16_t **exit_data);

  grub_efi_status_t
  (*exit) (grub_efi_handle_t image_handle,
	   grub_efi_status_t exit_status,
	   grub_efi_uintn_t exit_data_size,
	   grub_efi_char16_t *exit_data) __attribute__((noreturn));

  grub_efi_status_t
  (*unload_image) (grub_efi_handle_t image_handle);

  grub_efi_status_t
  (*exit_boot_services) (grub_efi_handle_t image_handle,
			 grub_efi_uintn_t map_key);

  grub_efi_status_t
  (*get_next_monotonic_count) (grub_efi_uint64_t *count);

  grub_efi_status_t
  (*stall) (grub_efi_uintn_t microseconds);

  grub_efi_status_t
  (*set_watchdog_timer) (grub_efi_uintn_t timeout,
			 grub_efi_uint64_t watchdog_code,
			 grub_efi_uintn_t data_size,
			 grub_efi_char16_t *watchdog_data);

  grub_efi_status_t
  (*connect_controller) (grub_efi_handle_t controller_handle,
			 grub_efi_handle_t *driver_image_handle,
			 grub_efi_device_path_protocol_t *remaining_device_path,
			 grub_efi_boolean_t recursive);

  grub_efi_status_t
  (*disconnect_controller) (grub_efi_handle_t controller_handle,
			    grub_efi_handle_t driver_image_handle,
			    grub_efi_handle_t child_handle);

  grub_efi_status_t
  (*open_protocol) (grub_efi_handle_t handle,
		    grub_efi_guid_t *protocol,
		    void **interface,
		    grub_efi_handle_t agent_handle,
		    grub_efi_handle_t controller_handle,
		    grub_efi_uint32_t attributes);

  grub_efi_status_t
  (*close_protocol) (grub_efi_handle_t handle,
		     grub_efi_guid_t *protocol,
		     grub_efi_handle_t agent_handle,
		     grub_efi_handle_t controller_handle);

  grub_efi_status_t
  (*open_protocol_information) (grub_efi_handle_t handle,
				grub_efi_guid_t *protocol,
				grub_efi_open_protocol_information_entry_t **entry_buffer,
				grub_efi_uintn_t *entry_count);

  grub_efi_status_t
  (*protocols_per_handle) (grub_efi_handle_t handle,
			   grub_efi_guid_t ***protocol_buffer,
			   grub_efi_uintn_t *protocol_buffer_count);

  grub_efi_status_t
  (*locate_handle_buffer) (grub_efi_locate_search_type_t search_type,
			   grub_efi_guid_t *protocol,
			   void *search_key,
			   grub_efi_uintn_t *no_handles,
			   grub_efi_handle_t **buffer);

  grub_efi_status_t
  (*locate_protocol) (grub_efi_guid_t *protocol,
		      void *registration,
		      void **interface);

  grub_efi_status_t
  (*install_multiple_protocol_interfaces) (grub_efi_handle_t *handle, ...);

  grub_efi_status_t
  (*uninstall_multiple_protocol_interfaces) (grub_efi_handle_t handle, ...);

  grub_efi_status_t
  (*calculate_crc32) (void *data,
		      grub_efi_uintn_t data_size,
		      grub_efi_uint32_t *crc32);

  void
  (*copy_mem) (void *destination, void *source, grub_efi_uintn_t length);

  void
  (*set_mem) (void *buffer, grub_efi_uintn_t size, grub_efi_uint8_t value);
};
typedef struct grub_efi_boot_services grub_efi_boot_services_t;

struct grub_efi_runtime_services
{
  grub_efi_table_header_t hdr;

  grub_efi_status_t 
  (*get_time) (grub_efi_time_t *time,
	       grub_efi_time_capabilities_t *capabilities);

  grub_efi_status_t
  (*set_time) (grub_efi_time_t *time);

  grub_efi_status_t
  (*get_wakeup_time) (grub_efi_boolean_t *enabled,
		      grub_efi_boolean_t *pending,
		      grub_efi_time_t *time);

  grub_efi_status_t
  (*set_wakeup_time) (grub_efi_boolean_t enabled,
		      grub_efi_time_t *time);

  grub_efi_status_t
  (*set_virtual_address_map) (grub_efi_uintn_t memory_map_size,
			      grub_efi_uintn_t descriptor_size,
			      grub_efi_uint32_t descriptor_version,
			      grub_efi_memory_descriptor_t *virtual_map);

  grub_efi_status_t
  (*convert_pointer) (grub_efi_uintn_t debug_disposition, void **address);

  grub_efi_status_t
  (*get_variable) (grub_efi_char16_t *variable_name,
		   grub_efi_guid_t *vendor_guid,
		   grub_efi_uint32_t *attributes,
		   grub_efi_uintn_t *data_size,
		   void *data);

  grub_efi_status_t
  (*get_next_variable_name) (grub_efi_uintn_t *variable_name_size,
			     grub_efi_char16_t *variable_name,
			     grub_efi_guid_t *vendor_guid);

  grub_efi_status_t
  (*set_variable) (grub_efi_char16_t *variable_name,
		   grub_efi_guid_t *vendor_guid,
		   grub_efi_uint32_t attributes,
		   grub_efi_uintn_t data_size,
		   void *data);

  grub_efi_status_t
  (*get_next_high_monotonic_count) (grub_efi_uint32_t *high_count);

  void
  (*reset_system) (grub_efi_reset_type_t reset_type,
		   grub_efi_status_t reset_status,
		   grub_efi_uintn_t data_size,
		   grub_efi_char16_t *reset_data);
};
typedef struct grub_efi_runtime_services grub_efi_runtime_services_t;

struct grub_efi_configuration_table
{
  grub_efi_guid_t vendor_guid;
  void *vendor_table;
};
typedef struct grub_efi_configuration_table grub_efi_configuration_table_t;

struct grub_efi_simple_input_interface
{
  grub_efi_status_t
  (*reset) (struct grub_efi_simple_input_interface *this,
	    grub_efi_boolean_t extended_verification);
  
  grub_efi_status_t
  (*read_key_stroke) (struct grub_efi_simple_input_interface *this,
		      grub_efi_input_key_t *key);
  
  grub_efi_event_t wait_for_key;
};
typedef struct grub_efi_simple_input_interface grub_efi_simple_input_interface_t;

struct grub_efi_simple_text_output_interface
{
  grub_efi_status_t
  (*reset) (struct grub_efi_simple_text_output_interface *this,
	    grub_efi_boolean_t extended_verification);

  grub_efi_status_t
  (*output_string) (struct grub_efi_simple_text_output_interface *this,
		    grub_efi_char16_t *string);

  grub_efi_status_t
  (*test_string) (struct grub_efi_simple_text_output_interface *this,
		  grub_efi_char16_t *string);

  grub_efi_status_t
  (*query_mode) (struct grub_efi_simple_text_output_interface *this,
		 grub_efi_uintn_t mode_number,
		 grub_efi_uintn_t *columns,
		 grub_efi_uintn_t *rows);

  grub_efi_status_t 
  (*set_mode) (struct grub_efi_simple_text_output_interface *this,
	       grub_efi_uintn_t mode_number);

  grub_efi_status_t
  (*set_attributes) (struct grub_efi_simple_text_output_interface *this,
		     grub_efi_uintn_t attribute);

  grub_efi_status_t
  (*clear_screen) (struct grub_efi_simple_text_output_interface *this);

  grub_efi_status_t
  (*set_cursor_position) (struct grub_efi_simple_text_output_interface *this,
			  grub_efi_uintn_t column,
			  grub_efi_uintn_t row);

  grub_efi_status_t
  (*enable_cursor) (struct grub_efi_simple_text_output_interface *this,
		    grub_efi_boolean_t visible);
  
  grub_efi_simple_text_output_mode_t *mode;
};
typedef struct grub_efi_simple_text_output_interface grub_efi_simple_text_output_interface_t;

struct grub_efi_system_table
{
  grub_efi_table_header_t hdr;
  grub_efi_char16_t *firmware_vendor;
  grub_efi_uint32_t firmware_revision;
  grub_efi_handle_t console_in_handler;
  grub_efi_simple_input_interface_t *con_in;
  grub_efi_handle_t console_out_handler;
  grub_efi_simple_text_output_interface_t *con_out;
  grub_efi_handle_t standard_error_handle;
  grub_efi_simple_text_output_interface_t *std_err;
  grub_efi_runtime_services_t *runtime_services;
  grub_efi_boot_services_t *boot_services;
  grub_efi_uintn_t num_table_entries;
  grub_efi_configuration_table_t *configuration_table;
};
typedef struct grub_efi_system_table  grub_efi_system_table_t;

#endif /* ! GRUB_EFI_API_HEADER */