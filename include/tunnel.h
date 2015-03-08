/* FUNCTION
 *      tun_alloc
 * 
 * DESCRIPTION
 *      Returns a file descriptor for the TUN device.
 * 
 * PARAMETERS
 *      device      Name of the device (e.g. "tun77").
 * 
 * RETURN VALUES
 *      Returns the file descriptor on success or a negative value on failure.
 */
int tun_alloc(char* device);
