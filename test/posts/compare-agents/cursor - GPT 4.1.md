# Review implementation plan for prisma schema
_Exported on 7/21/2025 at 14:04:37 MDT from Cursor (1.2.2)_

---

**User**


Can you look over the "Implenetation Plan" document, and see if we're missing anything from this prisma schema?

---

**Cursor**

Let's compare the entities and fields in your `prisma/schema.prisma` with the requirements and deliverables in the "Implementation Plan" to ensure nothing is missing.

## Entities in Prisma Schema

- **Artist**: id, email, passwordHash, fullName, profilePic, memberships, artworks, notifications, createdAt, resetToken, resetTokenExpiry
- **Group**: id, name, memberships, events, createdAt, Invitation
- **GroupMembership**: id, artist, group, isAdmin, createdAt
- **Event**: id, group, phase, submissionLimit, artworks, matches, createdAt
- **Artwork**: id, artist, event, title, description, medium, dimensions, year, images, status, createdAt, Match1, Match2
- **Match**: id, event, artwork1, artwork2, status, createdAt
- **Invitation**: id, group, email, token, status, createdAt
- **Notification**: id, artist, type, message, read, createdAt

## Entities/Features in Implementation Plan

- **Artist/User**: Account creation, password reset, profile, notifications
- **Group**: Management, admin/super-admin, invitations
- **Event**: Lifecycle (submission, voting, closed, archived), phase transitions, submission limits
- **Artwork**: Submission, portfolio, event association, images, status, flagging
- **Match**: Mutual likes, match notifications/history
- **Invitation**: Token, status, email
- **Notification**: In-app, match, event, flag, other
- **Authentication**: Password hashing, session management, invitation token validation, password reset
- **Admin/Moderation**: Flagging, user removal
- **Voting**: Tindr-style, summary/finalize, mutual likes
- **Image Upload**: Local file system, validation, resizing
- **Legal/Privacy**: Data retention, user data export/deletion

---

## Comparison & Gaps

### 1. **User Roles / Permissions**
- The schema has `isAdmin` on `GroupMembership`, which covers group-level admin.
- There is no global "super-admin" or site-wide admin role. If you need a global admin (not just per-group), you may want to add a field to `Artist` (e.g., `isSuperAdmin Boolean @default(false)`).

### 2. **Event Phases**
- `Event.phase` is a `String` (open, voting, closed, archived). Consider using an `enum` for type safety.

### 3. **Voting**
- There is no explicit model for "votes" (i.e., how users vote on artworks). If you want to track individual votes (for auditing, analytics, or to prevent double-voting), you may need a `Vote` model:
  ```prisma
  model Vote {
    id        String   @id @default(uuid())
    event     Event    @relation(fields: [eventId], references: [id])
    eventId   String
    voter     Artist   @relation(fields: [voterId], references: [id])
    voterId   String
    artwork   Artwork  @relation(fields: [artworkId], references: [id])
    artworkId String
    value     Int      // or Boolean, or enum for like/dislike
    createdAt DateTime @default(now())
    @@unique([eventId, voterId, artworkId])
  }
  ```

### 4. **Flagging**
- `ArtworkStatus` includes `FLAGGED`, but there is no model for flag reports (e.g., who flagged, why, when). If you want to track flagging actions, consider a `Flag` model:
  ```prisma
  model Flag {
    id        String   @id @default(uuid())
    artwork   Artwork  @relation(fields: [artworkId], references: [id])
    artworkId String
    reporter  Artist   @relation(fields: [reporterId], references: [id])
    reporterId String
    reason    String?
    createdAt DateTime @default(now())
  }
  ```

### 5. **Image Uploads**
- `Artwork.images` is a `Json` field (array of file paths/URLs). This is flexible, but if you want to track image metadata (uploader, upload date, etc.), you might want an `Image` model.

### 6. **Audit Logging / History**
- No explicit audit log for admin actions, deletions, or moderation. If you need to track these for compliance, consider an `AuditLog` model.

### 7. **Legal/Privacy**
- No explicit model for user data export/deletion requests, but this can be handled outside the schema.

### 8. **Notifications**
- The schema supports in-app notifications, but not email notification tracking (e.g., delivery status). If you want to track sent emails, consider an `EmailLog` model.

---

## Summary Table

| Feature/Entity         | In Schema? | Notes/Recommendation                                      |
|------------------------|:----------:|-----------------------------------------------------------|
| Artist/User            | Yes        |                                                           |
| Group                  | Yes        |                                                           |
| Group Admin            | Yes        | Per-group only; add global admin if needed                |
| Event                  | Yes        | Consider enum for phase                                   |
| Artwork                | Yes        |                                                           |
| Match                  | Yes        |                                                           |
| Invitation             | Yes        |                                                           |
| Notification           | Yes        |                                                           |
| Voting                 | No         | Add `Vote` model for tracking votes                       |
| Flagging               | Partial    | Add `Flag` model for flag reports                         |
| Image Upload           | Partial    | Add `Image` model if you want metadata                    |
| Audit Log              | No         | Add if you need admin/moderation history                  |
| Email Log              | No         | Add if you want to track email notifications              |
| Legal/Privacy          | N/A        | Can be handled outside schema                             |

---

## Recommendations

- **Add a `Vote` model** to track voting actions.
- **Add a `Flag` model** to track flagging of artworks.
- **Consider an `Image` model** if you want more than just file paths.
- **Consider an `AuditLog` model** for admin/moderation actions.
- **Add a global admin field** to `Artist` if you need site-wide admin.
- **Consider using enums** for fields like `Event.phase` for type safety.

Would you like to see example Prisma models for any of these recommendations?

```prisma
  model Vote {
    id        String   @id @default(uuid())
    event     Event    @relation(fields: [eventId], references: [id])
    eventId   String
    voter     Artist   @relation(fields: [voterId], references: [id])
    voterId   String
    artwork   Artwork  @relation(fields: [artworkId], references: [id])
    artworkId String
    value     Int      // or Boolean, or enum for like/dislike
    createdAt DateTime @default(now())
    @@unique([eventId, voterId, artworkId])
  }
```

```prisma
  model Flag {
    id        String   @id @default(uuid())
    artwork   Artwork  @relation(fields: [artworkId], references: [id])
    artworkId String
    reporter  Artist   @relation(fields: [reporterId], references: [id])
    reporterId String
    reason    String?
    createdAt DateTime @default(now())
  }
```

---

**User**

please make any recommended changes

---

**Cursor**

```prisma
// ... existing code ...
enum EventPhase {
  OPEN
  VOTING
  CLOSED
  ARCHIVED
}
// ... existing code ...
model Artist {
    id            String            @id @default(uuid())
    email         String            @unique
    passwordHash  String
    fullName      String
    profilePic    String? // URL or file path
    memberships   GroupMembership[]
    artworks      Artwork[]
    notifications Notification[]
    createdAt     DateTime          @default(now())
    resetToken   String? // for password reset
    resetTokenExpiry DateTime?
    isSuperAdmin Boolean @default(false) // global admin
}
// ... existing code ...
model Event {
    id              String    @id @default(uuid())
    group           Group     @relation(fields: [groupId], references: [id])
    groupId         String
    phase           EventPhase // open, voting, closed, archived
    submissionLimit Int
    artworks        Artwork[]
    matches         Match[]
    createdAt       DateTime  @default(now())
    votes           Vote[]
}
// ... existing code ...
model Vote {
    id        String   @id @default(uuid())
    event     Event    @relation(fields: [eventId], references: [id])
    eventId   String
    voter     Artist   @relation(fields: [voterId], references: [id])
    voterId   String
    artwork   Artwork  @relation(fields: [artworkId], references: [id])
    artworkId String
    value     Int      // 1 = like, 0 = skip/dislike, or use enum if needed
    createdAt DateTime @default(now())
    @@unique([eventId, voterId, artworkId])
}

model Flag {
    id         String   @id @default(uuid())
    artwork    Artwork  @relation(fields: [artworkId], references: [id])
    artworkId  String
    reporter   Artist   @relation(fields: [reporterId], references: [id])
    reporterId String
    reason     String?
    createdAt  DateTime @default(now())
}

model AuditLog {
    id         String   @id @default(uuid())
    actor      Artist?  @relation(fields: [actorId], references: [id])
    actorId    String?
    action     String
    targetType String
    targetId   String
    details    String?
    createdAt  DateTime @default(now())
}

model EmailLog {
    id         String   @id @default(uuid())
    to         String
    subject    String
    body       String?
    status     String // sent, failed, etc.
    error      String?
    createdAt  DateTime @default(now())
}
// ... existing code ...
```

I've updated your Prisma schema with the following improvements:

1. **EventPhase enum**: Added and used for `Event.phase` for type safety.
2. **isSuperAdmin**: Added to `Artist` for global admin capability.
3. **Vote model**: Added to track voting actions, with relations to `Event`, `Artist` (voter), and `Artwork`.
4. **Flag model**: Added to track flagging of artworks, with relations to `Artwork` and `Artist` (reporter).
5. **AuditLog model**: Added for tracking admin/moderation actions.
6. **EmailLog model**: Added for tracking sent emails and their status.

These changes ensure your schema fully supports the features and requirements outlined in your Implementation Plan, including voting, flagging, global admin, and audit/email logging.

You should now run:
```sh
npx prisma migrate dev --name add-vote-flag-admin-auditlog
```
to apply these changes to your database.

If you need example queries or CRUD functions for these new models, let me know!

```shellscript
npx prisma migrate dev --name add-vote-flag-admin-auditlog
```

