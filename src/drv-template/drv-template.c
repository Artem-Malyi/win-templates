//
// drv-template.c
//

//#undef _WIN32_WINNT
//#define _WIN32_WINNT 0x0500

#include <wdm.h>

#define DRIVER_NAME         L"drv-template"
#define IOCTL_PASS_INFO     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) // 0x222000

// Comment the following symbol in order to switch off debug output.
#define DEBUG_OUTPUT_KM

#ifdef DEBUG_OUTPUT_KM
    #define LOG_OUTPUT_KM(fmt, ...) DbgPrint("drv-template: %s - " fmt "\r\n", __FUNCTION__, __VA_ARGS__)
#else 
    #define LOG_OUTPUT_KM(...)
#endif

NTSTATUS TmplDispatchCreate(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
NTSTATUS TmplDispatchClose(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
NTSTATUS TmplDispatchDeviceControl(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
VOID     TmplDriverUnload(IN PDRIVER_OBJECT pDriverObject);


NTSTATUS TmplDriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{
    NTSTATUS        	status = STATUS_SUCCESS;
    UNICODE_STRING  	usDeviceName = { 0 };
    UNICODE_STRING  	usSymlinkName = { 0 };
    PDEVICE_OBJECT  	pDeviceObject = NULL;

    LOG_OUTPUT_KM("Entering");

    RtlInitUnicodeString(&usDeviceName, L"\\Device\\" DRIVER_NAME);
    status = IoCreateDevice(pDriverObject, 0, &usDeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
    if (!NT_SUCCESS(status))
        return status;

    pDriverObject->MajorFunction[IRP_MJ_CREATE] = TmplDispatchCreate;
    pDriverObject->MajorFunction[IRP_MJ_CLOSE] = TmplDispatchClose;
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = TmplDispatchDeviceControl;
    pDriverObject->DriverUnload = TmplDriverUnload;

    RtlInitUnicodeString(&usSymlinkName, L"\\DosDevices\\" DRIVER_NAME);
    status = IoCreateSymbolicLink(&usSymlinkName, &usDeviceName);
    if (!NT_SUCCESS(status))
        IoDeleteDevice(pDeviceObject);

    return status;
}


VOID TmplDriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
    PDEVICE_OBJECT pDeviceObject = pDriverObject->DeviceObject;
    UNICODE_STRING usSymlinkName = { 0 };

    LOG_OUTPUT_KM("Entering");

    RtlInitUnicodeString(&usSymlinkName, L"\\DosDevices\\" DRIVER_NAME);
    IoDeleteSymbolicLink(&usSymlinkName);
    if (pDeviceObject != NULL)
        IoDeleteDevice(pDeviceObject);
}


NTSTATUS TmplDispatchClose(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
    NTSTATUS status = STATUS_SUCCESS;

    LOG_OUTPUT_KM("Entering");

    pIrp->IoStatus.Status = status;
    pIrp->IoStatus.Information = 0;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return status;
}


NTSTATUS TmplDispatchCreate(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
    NTSTATUS status = STATUS_SUCCESS;

    LOG_OUTPUT_KM("Entering");

    pIrp->IoStatus.Status = status;
    pIrp->IoStatus.Information = 0;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return status;
}


NTSTATUS TmplDispatchDeviceControl(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
    PIO_STACK_LOCATION  pIosl = NULL;
    NTSTATUS            status = STATUS_SUCCESS;
    ULONG               ulInBufLength = 0;
    ULONG               ulOutBufLength = 0;

    LOG_OUTPUT_KM("entering");

    pIosl = IoGetCurrentIrpStackLocation(pIrp);
    ulInBufLength = pIosl->Parameters.DeviceIoControl.InputBufferLength;
    ulOutBufLength = pIosl->Parameters.DeviceIoControl.OutputBufferLength;

    switch (pIosl->Parameters.DeviceIoControl.IoControlCode)
    {
    case IOCTL_PASS_INFO:
        __try
        {
            //
            // Place your user-mode info handling code here
            //
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            LOG_OUTPUT_KM("SEH exception occurred");
        }
        break;
    default:
        pIrp->IoStatus.Information = 0;
        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return status;
}