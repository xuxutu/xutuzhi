# USB 鼠标设备驱动

## struct usb_device_id 

```
456struct usb_device_id {
457	/* which fields to match against? */
458	u16 match_flags;
459
460	/* Used for product specific matches; range is inclusive */
461	u16 idVendor;
462	u16 idProduct;
463	u16 bcdDevice_lo;
464	u16 bcdDevice_hi;
465
466	/* Used for device class matches */
467	u8 bDeviceClass;
468	u8 bDeviceSubClass;
469	u8 bDeviceProtocol;
470
471	/* Used for interface class matches */
472	u8 bInterfaceClass;
473	u8 bInterfaceSubClass;
474	u8 bInterfaceProtocol;
475
476	/* Used for vendor-specific interface matches */
477	u8 bInterfaceNumber;
478
479	/* not matched against */
480	ulong driver_info;
481};
482
```



## 1.usb_mouse_driver 

```
225static struct usb_driver usb_mouse_driver = {
226	.name		= "usbmouse",
227	.probe		= usb_mouse_probe,
228	.disconnect	= usb_mouse_disconnect,
229	.id_table	= usb_mouse_id_table,
230};

```

```
1204struct usb_driver {
1205	const char *name;
1206
1207	int (*probe) (struct usb_interface *intf,
1208		      const struct usb_device_id *id);
1209
1210	void (*disconnect) (struct usb_interface *intf);
1211
1212	int (*unlocked_ioctl) (struct usb_interface *intf, unsigned int code,
1213			void *buf);
1214
1215	int (*suspend) (struct usb_interface *intf, pm_message_t message);
1216	int (*resume) (struct usb_interface *intf);
1217	int (*reset_resume)(struct usb_interface *intf);
1218
1219	int (*pre_reset)(struct usb_interface *intf);
1220	int (*post_reset)(struct usb_interface *intf);
1221
1222	const struct usb_device_id *id_table;
1223	const struct attribute_group **dev_groups;
1224
1225	struct usb_dynids dynids;
1226	struct usbdrv_wrap drvwrap;
1227	unsigned int no_dynamic_id:1;
1228	unsigned int supports_autosuspend:1;
1229	unsigned int disable_hub_initiated_lpm:1;
1230	unsigned int soft_unbind:1;
1231
1232	ANDROID_KABI_RESERVE(1);
1233	ANDROID_KABI_RESERVE(2);
1234	ANDROID_KABI_RESERVE(3);
1235	ANDROID_KABI_RESERVE(4);
1236};
```



### module_usb_driver

```
module_usb_driver(usb_mouse_driver);
```

```
/**
1310 * module_usb_driver() - Helper macro for registering a USB driver
1311 * @__usb_driver: usb_driver struct
1312 *
1313 * Helper macro for USB drivers which do not do anything special in module
1314 * init/exit. This eliminates a lot of boilerplate. Each module may only
1315 * use this macro once, and calling it replaces module_init() and module_exit()
1316 */
1317#define module_usb_driver(__usb_driver) \
1318	module_driver(__usb_driver, usb_register, \
1319		       usb_deregister)
```

## 2.usb_register 

```
943int usb_register_driver(struct usb_driver *new_driver, struct module *owner,
944			const char *mod_name)
945{
946	int retval = 0;
947
948	if (usb_disabled())
949		return -ENODEV;
950
951	new_driver->drvwrap.for_devices = 0;
952	new_driver->drvwrap.driver.name = new_driver->name;
953	new_driver->drvwrap.driver.bus = &usb_bus_type;
954	new_driver->drvwrap.driver.probe = usb_probe_interface;
955	new_driver->drvwrap.driver.remove = usb_unbind_interface;
956	new_driver->drvwrap.driver.owner = owner;
957	new_driver->drvwrap.driver.mod_name = mod_name;
958	new_driver->drvwrap.driver.dev_groups = new_driver->dev_groups;
959	spin_lock_init(&new_driver->dynids.lock);
960	INIT_LIST_HEAD(&new_driver->dynids.list);
961
962	retval = driver_register(&new_driver->drvwrap.driver);
963	if (retval)
964		goto out;
965
966	retval = usb_create_newid_files(new_driver);
967	if (retval)
968		goto out_newid;
969
970	pr_info("%s: registered new interface driver %s\n",
971			usbcore_name, new_driver->name);
972
973out:
974	return retval;
975
976out_newid:
977	driver_unregister(&new_driver->drvwrap.driver);
978
979	printk(KERN_ERR "%s: error %d registering interface "
980			"	driver %s\n",
981			usbcore_name, retval, new_driver->name);
982	goto out;
983}
```

## 3.driver_register

```
146int driver_register(struct device_driver *drv)
147{
148	int ret;
149	struct device_driver *other;
150
151	if (!drv->bus->p) {
152		pr_err("Driver '%s' was unable to register with bus_type '%s' because the bus was not initialized.\n",
153			   drv->name, drv->bus->name);
154		return -EINVAL;
155	}
156
157	if ((drv->bus->probe && drv->probe) ||
158	    (drv->bus->remove && drv->remove) ||
159	    (drv->bus->shutdown && drv->shutdown))
160		printk(KERN_WARNING "Driver '%s' needs updating - please use "
161			"bus_type methods\n", drv->name);
162
163	other = driver_find(drv->name, drv->bus);
164	if (other) {
165		printk(KERN_ERR "Error: Driver '%s' is already registered, "
166			"aborting...\n", drv->name);
167		return -EBUSY;
168	}
169
170	ret = bus_add_driver(drv);
171	if (ret)
172		return ret;
173	ret = driver_add_groups(drv, drv->groups);
174	if (ret) {
175		bus_remove_driver(drv);
176		return ret;
177	}
178	kobject_uevent(&drv->p->kobj, KOBJ_ADD);
179
180	return ret;
181}
```

## 4.usb_mouse_probe

```
1.注册input设备
2.判断是否符合usbspec
3.初始化urb
```

