C program for extracting anonymous edits from the Wikipedia database backup dumps: http://dumps.wikimedia.org/backup-index.html

It extracts those edits and creates PostgreSQL-readable file for a `COPY .. WITH BINARY` command, so we can quickly load them into a database and search among them.

## Dependencies

Program depends on a `libxml` library.

## Notes

1. WIP;
2. Just an educational project;
3. Still have no ideas how to cook C properly.
