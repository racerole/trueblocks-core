package cacheNew

import (
	"bytes"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew/locations"
)

// prepare performs read/write setup
func prepare(value Locator, id string, options *StoreOptions) (loc Storer, itemPath string, err error) {
	loc, err = options.location()
	if err != nil {
		return
	}
	itemPath, err = cachePath(value, id)
	return
}

// Write saves value to a location defined by options.Location. If options is nil,
// then FileSystem is used. The value has to implement Locator interface, which
// provides information about in-cache path and ID.
func Write(value Locator, options *StoreOptions) (err error) {
	loc, itemPath, err := prepare(value, "", options)
	if err != nil {
		return
	}

	readerWriter, err := loc.Writer(itemPath)
	if err != nil {
		return
	}
	defer readerWriter.Close()

	buffer := new(bytes.Buffer)
	item := NewItem(buffer)
	if err = item.Encode(value); err != nil {
		return
	}
	_, err = buffer.WriteTo(readerWriter)

	return
}

// Read retrieves value of ID id from a location defined by options.Location. If options is nil,
// then FileSystem is used. The value has to implement Locator interface, which
// provides information about in-cache path
func Read(value Locator, id string, options *StoreOptions) (err error) {
	loc, itemPath, err := prepare(value, id, options)
	if err != nil {
		return
	}

	readerWriter, err := loc.Reader(itemPath)
	if err != nil {
		return
	}
	defer readerWriter.Close()

	buffer := new(bytes.Buffer)
	if _, err = buffer.ReadFrom(readerWriter); err != nil {
		return
	}

	item := NewItem(buffer)
	return item.Decode(value)
}

func Stat(value Locator, options *StoreOptions) (result *locations.ItemInfo, err error) {
	// TODO: finish this func. Also, make Store a struct, so no need to call prepare
	// every time.
	loc, itemPath, err := prepare(value, "", options)
	if err != nil {
		return
	}
	return loc.Stat(itemPath)
}

func Remove(value Locator, options *StoreOptions) error {
	loc, itemPath, err := prepare(value, "", options)
	if err != nil {
		return err
	}
	return loc.Remove(itemPath)
}
