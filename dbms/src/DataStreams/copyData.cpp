#include <DB/DataStreams/RowInputStreamFromBlockInputStream.h>
#include <DB/DataStreams/BlockInputStreamFromRowInputStream.h>

#include <DB/DataStreams/copyData.h>


namespace DB
{

void copyData(IBlockInputStream & from, IBlockOutputStream & to)
{
	from.readPrefix();
	to.writePrefix();

	while (Block block = from.read())
		to.write(block);

	from.readSuffix();
	to.writeSuffix();
}


void copyData(IRowInputStream & from, IRowOutputStream & to)
{
	from.readPrefix();
	to.writePrefix();

	bool first = true;
	while (1)
	{
		if (first)
			first = false;
		else
		{
			from.readRowBetweenDelimiter();
			to.writeRowBetweenDelimiter();
		}
		
		Row row = from.read();
		if (row.empty())
			break;
		to.write(row);
	}

	from.readSuffix();
	to.writeSuffix();
}


void copyData(IBlockInputStream & from, IRowOutputStream & to)
{
	RowInputStreamFromBlockInputStream row_input(from.clone());
	copyData(row_input, to);
}


void copyData(IRowInputStream & from, IBlockOutputStream & to, const Block & sample)
{
	BlockInputStreamFromRowInputStream block_input(from.clone(), sample);
	copyData(block_input, to);
}

}
