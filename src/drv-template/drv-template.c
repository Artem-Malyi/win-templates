//
// drv-template.c
//

#include <wdm.h>

#ifdef _WIN64
    #ifdef _DEBUG
        #define DRIVER_NAME_A "drv-template64d"
    #else
        #define DRIVER_NAME_A "drv-template64"
    #endif
#else
    #ifdef _DEBUG
        #define DRIVER_NAME_A "drv-template32d"
    #else
        #define DRIVER_NAME_A "drv-template32"
    #endif
#endif

#define __T(x)      L##x
#define _T(x)       __T(x)
#define DRIVER_NAME _T(DRIVER_NAME_A)
#define IOCTL_PASS_INFO     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) // 0x222000

// Comment the following symbol in order to switch off debug output.
#define DEBUG_OUTPUT_KM

#ifdef DEBUG_OUTPUT_KM
    #define KMLOG(fmt, ...) DbgPrint(DRIVER_NAME_A ": %s - " fmt "\r\n", __FUNCTION__, __VA_ARGS__)
#else 
    #define KMLOG(...)
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

    KMLOG("Entering");

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

    KMLOG("Entering");

    RtlInitUnicodeString(&usSymlinkName, L"\\DosDevices\\" DRIVER_NAME);
    IoDeleteSymbolicLink(&usSymlinkName);
    if (pDeviceObject != NULL)
        IoDeleteDevice(pDeviceObject);
}


NTSTATUS TmplDispatchClose(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
    NTSTATUS status = STATUS_SUCCESS;

    KMLOG("Entering");

    pIrp->IoStatus.Status = status;
    pIrp->IoStatus.Information = 0;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return status;
}


NTSTATUS TmplDispatchCreate(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
    NTSTATUS status = STATUS_SUCCESS;

    KMLOG("Entering");

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

    KMLOG("Entering");

    pIosl = IoGetCurrentIrpStackLocation(pIrp);
    ulInBufLength = pIosl->Parameters.DeviceIoControl.InputBufferLength;
    ulOutBufLength = pIosl->Parameters.DeviceIoControl.OutputBufferLength;

    switch (pIosl->Parameters.DeviceIoControl.IoControlCode)
    {
    case IOCTL_PASS_INFO:
        __try
        {
            KMLOG("Handling info from user mode");
            //
            // Place your user-mode info handling code here
            //
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            KMLOG("SEH exception occurred");
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