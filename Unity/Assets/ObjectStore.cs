/// <summary>
/// Stores objects and allows access to them via an int.
/// This class is thread-safe.
/// </summary>
/// 
/// <author>
/// JacksonDunstan, http://JacksonDunstan.com/articles/3908
/// </author>
/// 
/// <license>
/// MIT
/// </license>
public static class ObjectStore
{
	// Stored objects. The first is always null.
	private static object[] objects;

	// Stack of available handles
	private static int[] handles;

	// Index of the next available handle
	private static int nextHandleIndex;

	/// <summary>
	/// Initialize the object storage and reset the handles
	/// </summary>
	/// 
	/// <param name="maxObjects">
	/// Maximum number of objects to store. Must be positive.
	/// </param>
	public static void Init(int maxObjects)
	{
		// Initialize the objects as all null plus room for the
		// first to always be null.
		objects = new object[maxObjects + 1];

		// Initialize the handles stack as 1, 2, 3, ...
		handles = new int[maxObjects];
		for (
			int i = 0, handle = maxObjects;
			i < maxObjects;
			++i, --handle)
		{
			handles[i] = handle;
		}
		nextHandleIndex = maxObjects - 1;
	}

	/// <summary>
	/// Store an object
	/// </summary>
	/// 
	/// <param name="obj">
	/// Object to store. This can be null.
	/// </param>
	/// 
	/// <returns>
	/// An handle to the stored object that can be used with
	/// <see cref="Get"/> and <see cref="Remove"/>. If
	/// <see cref="Init"/> has not yet been called, a
	/// <see cref="NullReferenceException"/> will be thrown.
	/// </returns>
	public static int Store(object obj)
	{
		// Null is always zero
		if (object.ReferenceEquals(obj, null))
		{
			return 0;
		}
		
		lock (objects)
		{
			// Pop a handle off the stack
			int handle = handles[nextHandleIndex];
			nextHandleIndex--;

			// Store the object
			objects[handle] = obj;

			// Return the handle
			return handle;
		}
	}

	/// <summary>
	/// Get the object for a given handle
	/// </summary>
	/// 
	/// <param name="handle">
	/// Handle of the object to get. If this is less than zero
	/// or greater than the maximum number of objects passed to
	/// <see cref="Init"/>, this function will throw an
	/// <see cref="ArrayIndexOutOfBoundsException"/>. If this
	/// is zero, not a handle returned by <see cref="Store"/>,
	/// a handle returned by a call to <see cref="Store"/> with
	/// a null parameter, or a handle passed to
	/// <see cref="Remove"/> and not subsequently returned by
	/// <see cref="Store"/>, this function will return null. If
	/// <see cref="Init"/> has not yet been called, a
	/// <see cref="NullReferenceException"/> will be thrown.
	/// </param>
	public static object Get(int handle)
	{
		return objects[handle];
	}

	/// <summary>
	/// Remove a stored object
	/// </summary>
	/// 
	/// <param name="handle">
	/// Handle of the object to Remove. If this is less than
	/// zero or greater than the maximum number of objects
	/// passed to <see cref="Init"/>, this function will throw
	/// an <see cref="ArrayIndexOutOfBoundsException"/>. The
	/// handle may be be reused. If <see cref="Init"/> has not
	/// yet been called, a <see cref="NullReferenceException"/>
	/// will be thrown.
	/// </param>
	public static void Remove(int handle)
	{
		if (handle != 0)
		{
			lock (objects)
			{
				// Forget the object
				objects[handle] = null;

				// Push the handle onto the stack
				nextHandleIndex++;
				handles[nextHandleIndex] = handle;
			}
		}
	}
}